#include <DLX.hpp>

#include <exceptions.hpp>

#include <algorithm>
#include <ostream>

namespace DLX
{
Matrix::Matrix(int nColumns)
    :m_nColumns(nColumns), m_nRows(0)
{
    m_matrixHeader = m_storage.allocate<Header>();

    ColumnHeaderListElement* it = m_matrixHeader;
    for(int i=0; i<m_nColumns; ++i)
    {
        auto column_header = m_storage.allocate<ColumnHeader>();
        column_header->columnIndex = i;
        column_header->previousInColumn = column_header->nextInColumn = column_header;
        it->nextInHeaderList = column_header;
        column_header->previousInHeaderList = it;
        it = column_header;
    }
    it->nextInHeaderList = m_matrixHeader;
    m_matrixHeader->previousInHeaderList = it;
}

void Matrix::addRow(RowHeader const& row_header, std::vector<int> const& occupied_fields)
{
    MatrixElement* row_it = nullptr;
    MatrixElement* first_in_row = nullptr;
    auto column_header_it = static_cast<ColumnHeader*>(m_matrixHeader->nextInHeaderList);
    for(int i=0; i<m_nColumns; ++i)
    {
        if(std::find(begin(occupied_fields), end(occupied_fields), i) != end(occupied_fields))
        {
            column_header_it->columnCount += 1;
            auto new_entry = m_storage.allocate<MatrixElement>();
            new_entry->rowIndex = m_nRows;

            // link in column list
            {
                new_entry->columnHeader = column_header_it;
                auto previous_element = column_header_it->previousInColumn;
                auto next_element = column_header_it;
                previous_element->nextInColumn = new_entry;
                new_entry->previousInColumn = previous_element;
                next_element->previousInColumn = new_entry;
                new_entry->nextInColumn = next_element;
            }

            // link in row list
            {
                new_entry->previousInRow = row_it;
                if(row_it) { row_it->nextInRow = new_entry; } else { first_in_row = new_entry; }
                row_it = new_entry;
            }
        }
        column_header_it = static_cast<ColumnHeader*>(column_header_it->nextInHeaderList);
    }
    // link row list boundaries
    if(row_it) {
        row_it->nextInRow = first_in_row;
        first_in_row->previousInRow = row_it;
    }
    m_rowHeaders.push_back(row_header);
    ++m_nRows;
}

bool Matrix::isOccupied(int row, int col) const
{
    auto column_header_it = static_cast<ColumnHeader*>(m_matrixHeader->nextInHeaderList);
    for(int i=0; i<col; ++i) { column_header_it = static_cast<ColumnHeader*>(column_header_it->nextInHeaderList); }
    if(column_header_it->columnIndex != col) { PROTOCOL_VIOLATION("Invalid column index"); }

    auto column_it = column_header_it->nextInColumn;
    do {
        if(static_cast<MatrixElement*>(column_it)->rowIndex == row) { return true; }
        column_it = column_it->nextInColumn;
    } while(column_it != column_header_it);
    return false;
}

void Matrix::printMatrix(std::ostream& os, int pieceCount, int field_width,
                         RowHeaderUserDataPrinter const& pretty_printer, bool compact) const
{
    for(int i=0; i<m_nRows; ++i)
    {
        printRow(i, os, pieceCount, field_width, pretty_printer, compact);
        os << (compact ? "\n" : "\n\n");
    }
}

void Matrix::printRow(int rowIndex, std::ostream& os, int pieceCount, int field_width,
                      RowHeaderUserDataPrinter const& pretty_printer, bool compact) const
{
    if(pretty_printer) { pretty_printer(os, m_rowHeaders[rowIndex].UserData); }
    for(int j=0; j<m_nColumns; ++j)
    {
        if(!compact && ((j >= pieceCount)) && ((j - pieceCount) % field_width == 0)) { os << '\n'; }
        os << (isOccupied(rowIndex, j) ? '1' : '0') << ' ';
    }
}

Matrix Matrix::clone() const
{
    Matrix ret(m_nColumns);
    std::vector<int> occupied_fields;
    for(int i=0; i<m_nRows; ++i)
    {
        occupied_fields.clear();
        for(int j=0; j<m_nColumns; ++j)
        {
            if(isOccupied(i, j)) { occupied_fields.push_back(j); }
        }
        ret.addRow(m_rowHeaders[i], occupied_fields);
    }
    return ret;
}

ColumnHeader* Matrix::getHeaderWithFewestOccupants()
{
    ColumnHeader* ret = nullptr;
    auto column_header_it = m_matrixHeader->nextInHeaderList;
    while(column_header_it != m_matrixHeader) {
        if(!ret) {
            ret = static_cast<ColumnHeader*>(column_header_it);
        } else {
            auto candidate = static_cast<ColumnHeader*>(column_header_it);
            if(candidate->columnCount < ret->columnCount) { ret = candidate; }
        }
        column_header_it = column_header_it->nextInHeaderList;
    }
    return ret;
}

void Matrix::coverColumn(ColumnHeader* column_header)
{
    // unlink column header
    column_header->nextInHeaderList->previousInHeaderList = column_header->previousInHeaderList;
    column_header->previousInHeaderList->nextInHeaderList = column_header->nextInHeaderList;

    // traverse column elements
    auto column_it = column_header->nextInColumn;
    for(int i=0; i<column_header->columnCount; ++i)
    {
        auto matrix_element = static_cast<MatrixElement*>(column_it);
        // unlink all other elements on the same row from their column
        for(auto it = matrix_element->nextInRow; it != matrix_element; it = it->nextInRow)
        {
            it->nextInColumn->previousInColumn = it->previousInColumn;
            it->previousInColumn->nextInColumn = it->nextInColumn;
            it->columnHeader->columnCount -= 1;
        }
        column_it = column_it->nextInColumn;
    }
}

void Matrix::uncoverColumn(ColumnHeader* column_header)
{
    // traverse column elements
    auto column_it = column_header->previousInColumn;
    for(int i=0; i<column_header->columnCount; ++i)
    {
        auto matrix_element = static_cast<MatrixElement*>(column_it);
        // relink all other elements on the same row to their column
        for(auto it = matrix_element->previousInRow; it != matrix_element; it = it->previousInRow)
        {
            it->nextInColumn->previousInColumn = it;
            it->previousInColumn->nextInColumn = it;
            it->columnHeader->columnCount += 1;
        }
        column_it = column_it->previousInColumn;
    }

    // relink column header
    column_header->nextInHeaderList->previousInHeaderList = column_header;
    column_header->previousInHeaderList->nextInHeaderList = column_header;
}

Matrix::Solution Matrix::solve()
{
    PartialSolution partial_solution;
    std::vector<Solution> solutions;
    search(0, partial_solution, solutions, true);
    return (solutions.empty()) ? Solution() : solutions.back();
}

std::vector<Matrix::Solution> Matrix::solveAll()
{
    PartialSolution partial_solution;
    std::vector<Solution> solutions;
    search(0, partial_solution, solutions, false);
    return solutions;
}

Matrix::Solution Matrix::convertPartialSolutionToSolution(PartialSolution const& partial_solution) const
{
    Solution ret(partial_solution.size());
    std::transform(begin(partial_solution), end(partial_solution), begin(ret),
                   [](MatrixElement const* el) { return el->rowIndex; });
    return ret;
}

bool Matrix::search(int k, PartialSolution& partial_solution, std::vector<Solution>& solutions,
                    bool stopAfterFirstSolutionFound)
{
    if(m_matrixHeader->nextInHeaderList == m_matrixHeader)
    {
        // no more columns, we have a solution
        solutions.push_back(convertPartialSolutionToSolution(partial_solution));
        return true;
    }

    // chose an initial column -
    //  this corresponds to chosing a piece to place or a cell to fill
    ColumnHeader* c = getHeaderWithFewestOccupants();
    coverColumn(c);

    // iterate all rows for the chosen column -
    //  that is, iterate over all possibilities to place the piece / fill the cell
    bool found_solution = false;
    auto row_it = c->nextInColumn;
    while(row_it != c) {
        auto selected_element = static_cast<MatrixElement*>(row_it);
        partial_solution.push_back(selected_element);
        // iterate over all elements on the same row as the current element and cover their columns -
        //  that is, for the current placement, get all affected cell fields / pieces and remove them from
        //  the search tree
        auto column_it = selected_element->nextInRow;
        while(column_it != selected_element)
        {
            coverColumn(column_it->columnHeader);
            column_it = column_it->nextInRow;
        }

        found_solution = search(k+1, partial_solution, solutions, stopAfterFirstSolutionFound);
        partial_solution.pop_back();

        // undo the covering done above so we are ready to select a new element in the next iteration
        column_it = selected_element->previousInRow;
        while(column_it != row_it)
        {
            uncoverColumn(column_it->columnHeader);
            column_it = column_it->previousInRow;
        }
        row_it = row_it->nextInColumn;
        if (stopAfterFirstSolutionFound && found_solution) { break; }
    }
    // if we end up here without a found_solution, that means we exhausted the current sub-search tree
    uncoverColumn(c);

    return found_solution;
}

RowHeader const& Matrix::getRowHeader(int rowIndex)
{
    return m_rowHeaders.at(rowIndex);
}
}
