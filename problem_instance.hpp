#pragma once

#include <DLX.hpp>
#include <exceptions.hpp>
#include <polyomino.hpp>

#include <vector>

namespace Polyomino
{

struct FieldSize
{
    int x;
    int y;
};

template<typename Shape_T>
class ProblemInstance
{
    ProblemInstance(ProblemInstance const&)=delete;
    ProblemInstance& operator=(ProblemInstance const&)=delete;
public:
    ProblemInstance(FieldSize const& field_size)
        :m_fieldSize(field_size)
    {
        if ((m_fieldSize.x * m_fieldSize.y) % Degree<Shape_T>::value != 0) {
            PROTOCOL_VIOLATION("Invalid field size");
        }
    }

    void addPiece(Shape_T const& s)
    {
        if((m_pieces.size() + 1) > getRequiredPieceCount()) { PROTOCOL_VIOLATION("Too many pieces"); }
        m_pieces.push_back(s);
    }

    int getRequiredPieceCount() const
    {
        auto const field_area = m_fieldSize.x * m_fieldSize.y;
        return field_area / Degree<Shape_T>::value;
    }

    DLX::Matrix calculateProblemMatrix() const
    {
        if(m_pieces.size() != getRequiredPieceCount()) { PROTOCOL_VIOLATION("Not enough pieces to solve"); }
        int const field_area = m_fieldSize.x * m_fieldSize.y;
        int nColumns =  static_cast<int>(m_pieces.size()) + field_area;
        DLX::Matrix m(nColumns);
        int pieceCount = 0;
        std::vector<int> occupied_fields;
        for(auto const& piece : m_pieces)
        {
            for(int rot=0; rot<getRotations(piece); ++rot)
            {
                auto const placement = getPlacement(piece, rot);
                for(int x = 0; x < (m_fieldSize.x - placement.bound.x + 1); ++x)
                {
                    for(int y = 0; y < (m_fieldSize.y - placement.bound.y + 1); ++y)
                    {
                        occupied_fields.clear();
                        DLX::RowHeader row_header;
                        row_header.UserData = &m_pieces[pieceCount];

                        // traverse piece columns
                        int i = 0;
                        for(; i<static_cast<int>(m_pieces.size()); ++i)
                        {
                            if (i == pieceCount) { occupied_fields.push_back(i); }
                        }
                        // traverse field columns
                        for(int iy = 0; iy < m_fieldSize.y; ++iy)
                        {
                            for(int ix = 0; ix < m_fieldSize.x; ++ix)
                            {
                                for(auto const& p : placement.layout)
                                {
                                    if(x + p.x == ix && y + p.y == iy) { occupied_fields.push_back(i); break; }
                                }
                                ++i;
                            }
                        }
                        m.addRow(row_header, occupied_fields);
                    }
                }
            }
            ++pieceCount;
        }
        return m;
    }

    FieldSize getFieldSize() const
    {
        return m_fieldSize;
    }

    int getCurrentPieceCount() const
    {
        return static_cast<int>(m_pieces.size());
    }


private:
    FieldSize const m_fieldSize;
    std::vector<Shape_T> m_pieces;
};

}
