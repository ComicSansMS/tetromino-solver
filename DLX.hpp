#pragma once

#include <cstring>
#include <functional>
#include <iosfwd>
#include <memory>
#include <new>
#include <vector>

namespace DLX
{
    struct ColumnElement
    {
        ColumnElement* nextInColumn;
        ColumnElement* previousInColumn;

        ColumnElement()
            :nextInColumn(nullptr), previousInColumn(nullptr)
        {}
    };

    struct ColumnHeaderListElement : public ColumnElement
    {
        ColumnHeaderListElement* nextInHeaderList;
        ColumnHeaderListElement* previousInHeaderList;

        ColumnHeaderListElement()
            :nextInHeaderList(nullptr), previousInHeaderList(nullptr)
        {}
    };

    struct ColumnHeader: public ColumnHeaderListElement
    {
        int columnCount;
        int columnIndex;

        ColumnHeader()
            :columnCount(0), columnIndex(-1)
        {}
    };

    struct MatrixElement : public ColumnElement
    {
        MatrixElement* nextInRow;
        MatrixElement* previousInRow;
        ColumnHeader* columnHeader;
        int rowIndex;

        MatrixElement()
            :nextInRow(nullptr), previousInRow(nullptr), columnHeader(nullptr), rowIndex(-1)
        {}
    };

    struct Header : public ColumnHeaderListElement
    {
        Header()
        {}
    };

    /*! A row represents the placing of a single polyomino piece on the field.
     * The number of rows corresponds to the number of possible placements of the single pieces on the field.
     * Each row is contains one element for each polyomino piece and one element for each cell in the field.
     * The number of occupied elements in a row is always equal to the degree of the polyomino + 1 (one for the piece
     * and one for each cell occupied by the piece).
     */
    struct RowHeader
    {
        void const* UserData;

        RowHeader()
        {
            std::memset(this, 0, sizeof(*this));
        }
    };

    typedef std::function<void(std::ostream&, void const*)> RowHeaderUserDataPrinter;

    class Storage
    {
        Storage(Storage const&)=delete;
        Storage& operator=(Storage const&)=delete;
    private:
        struct Block
        {
            Block(std::size_t block_size) : memory(new char[block_size]) {}
            Block(Block&& rhs) : memory(std::move(rhs.memory)) {}
            ~Block() {}
            std::unique_ptr<char[]> memory;
        };
    public:
        Storage()
            :m_blockSize(1024*1024), m_offset(0), m_bytesWasted(0)
        {
            m_storage.push_back(Block(m_blockSize));
        }

        Storage(Storage&& rhs) : m_storage(std::move(rhs.m_storage)), m_blockSize(rhs.m_blockSize),
                                 m_offset(rhs.m_offset), m_bytesWasted(rhs.m_bytesWasted)
        {}

        template<typename T>
        T* allocate()
        {
            if(m_blockSize - m_offset < sizeof(T))
            {
                m_storage.push_back(Block(m_blockSize));
                m_bytesWasted += m_blockSize - m_offset;
                m_offset = 0;
            }
            auto mem = m_storage.back().memory.get() + m_offset;
            T* ret = new(mem) T();
            m_offset += sizeof(T);
            return ret;
        }

    private:
        std::vector<Block> m_storage;
        std::size_t const m_blockSize;
        std::size_t m_offset;
        std::size_t m_bytesWasted;
    };

    class Matrix
    {
    public:
        // the solution is given as a list of row indices
        typedef std::vector<int> Solution;
    public:
        Matrix(int nColumns);

        Matrix(Matrix&& rhs) : m_nColumns(rhs.m_nColumns), m_nRows(rhs.m_nRows), m_storage(std::move(rhs.m_storage)),
                               m_matrixHeader(rhs.m_matrixHeader), m_rowHeaders(std::move(rhs.m_rowHeaders))
        {}

        void addRow(RowHeader const& row_header, std::vector<int> const& occupied_fields);

        void printMatrix(std::ostream& os, int pieceCount, int field_width,
                         RowHeaderUserDataPrinter const& pretty_printer, bool compact) const;

        void printRow(int rowIndex, std::ostream& os, int pieceCount, int field_width,
                      RowHeaderUserDataPrinter const& pretty_printer, bool compact) const;

        Matrix clone() const;

        bool isOccupied(int row, int col) const;

        Solution solve();

        std::vector<Solution> solveAll();

        RowHeader const& getRowHeader(int rowIndex);

    private:
        typedef std::vector<MatrixElement const*> PartialSolution;

        Solution convertPartialSolutionToSolution(PartialSolution const& partial_solution) const;

        bool search(int k, PartialSolution& partial_solution, std::vector<Solution>& solutions,
                    bool stopAfterFirstSolutionFound);

        ColumnHeader* getHeaderWithFewestOccupants();

        void coverColumn(ColumnHeader* column_header);

        void uncoverColumn(ColumnHeader* column_header);

    private:
        int m_nColumns;
        int m_nRows;
        Storage m_storage;
        Header* m_matrixHeader;
        std::vector<RowHeader> m_rowHeaders;
    };
}
