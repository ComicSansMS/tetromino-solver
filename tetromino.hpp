#pragma once

#include <polyomino.hpp>

#include <iosfwd>

namespace Tetromino
{
    namespace OneSided
    {
        enum class Shape
        {
            I = 0,
            O,
            T,
            J,
            L,
            S,
            Z,
            END
        };

        std::ostream& operator<<(std::ostream& os, Shape const& s);
    }
}

namespace Polyomino
{
template<>
struct Degree<Tetromino::OneSided::Shape>
{
    enum Value_T { value = 4 };
};
}

namespace Tetromino
{
    namespace OneSided
    {
        using Placement = Polyomino::GenericPlacement<Shape>;

        inline Shape begin()
        {
            return Shape::I;
        }

        inline Shape end()
        {
            return Shape::END;
        }

        inline Shape inc(Shape s)
        {
            return (s == Shape::END) ? Shape::END : static_cast<Shape>(static_cast<int>(s) + 1);
        }

        inline int getRotations(Shape const& s)
        {
            static const int rotations[] ={ 2 /* I */, 1 /* O */, 4 /* T */, 4 /* J */,
                                            4 /* L */, 2 /* S */, 2 /* Z */ };
            return rotations[static_cast<int>(s)];
        }

        Placement getPlacement(Shape s, int rotation);
    }
}
