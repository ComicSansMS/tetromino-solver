#pragma once

#include <array>

namespace Polyomino
{

template<typename Shape_T>
struct Degree;

template<typename Shape_T>
struct GenericPlacement
{
    struct Bound {
        int x;
        int y;
    } bound;
    struct SinglePieceLayout {
        int x;
        int y;
    };
    std::array<SinglePieceLayout, Degree<Shape_T>::value> layout;
};

}
