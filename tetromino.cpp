#include <tetromino.hpp>

#include <exceptions.hpp>

#include <ostream>

namespace Tetromino
{
namespace OneSided
{

std::ostream& operator<<(std::ostream& os, Shape const& s)
{
    using Tetromino::OneSided::Shape;
    switch(s)
    {
        case Shape::I: return os << "I";
        case Shape::O: return os << "O";
        case Shape::T: return os << "T";
        case Shape::J: return os << "J";
        case Shape::L: return os << "L";
        case Shape::S: return os << "S";
        case Shape::Z: return os << "Z";
        default: return os << "<Invalid Shape>";
    }
}

Placement getPlacement(Shape s, int rotation)
{
    if (s == Shape::I) {
        if (rotation == 0) {
            // IIII
            return Placement {
                Placement::Bound{4, 1},
                {
                    Placement::SinglePieceLayout{0, 0},
                    Placement::SinglePieceLayout{1, 0},
                    Placement::SinglePieceLayout{2, 0},
                    Placement::SinglePieceLayout{3, 0}
                }
            };
        } else if(rotation == 1) {
            // I
            // I
            // I
            // I
            return Placement {
                Placement::Bound{1, 4},
                {
                    Placement::SinglePieceLayout{0, 0},
                    Placement::SinglePieceLayout{0, 1},
                    Placement::SinglePieceLayout{0, 2},
                    Placement::SinglePieceLayout{0, 3}
                }
            };
        } else {
            PROTOCOL_VIOLATION("Invalid rotation for Shape I");
        }
    } else if (s == Shape::O) {
        if (rotation == 0) {
            // OO
            // OO
            return Placement {
                Placement::Bound{2, 2},
                {
                    Placement::SinglePieceLayout{0, 0},
                    Placement::SinglePieceLayout{0, 1},
                    Placement::SinglePieceLayout{1, 0},
                    Placement::SinglePieceLayout{1, 1}
                }
            };
        } else {
            PROTOCOL_VIOLATION("Invalid rotation for Shape O");
        }
    } else if (s == Shape::T) {
        if (rotation == 0) {
            //  T
            // TTT
            return Placement {
                Placement::Bound{3, 2},
                {
                    Placement::SinglePieceLayout{1, 0},
                    Placement::SinglePieceLayout{0, 1},
                    Placement::SinglePieceLayout{1, 1},
                    Placement::SinglePieceLayout{2, 1}
                }
            };
        } else if(rotation == 1) {
            // TTT
            //  T
            return Placement {
                Placement::Bound{3, 2},
                {
                    Placement::SinglePieceLayout{0, 0},
                    Placement::SinglePieceLayout{1, 0},
                    Placement::SinglePieceLayout{2, 0},
                    Placement::SinglePieceLayout{1, 1}
                }
            };
        } else if(rotation == 2) {
            // T
            // TT
            // T
            return  Placement {
                Placement::Bound{2, 3},
                {
                    Placement::SinglePieceLayout{0, 0},
                    Placement::SinglePieceLayout{0, 1},
                    Placement::SinglePieceLayout{1, 1},
                    Placement::SinglePieceLayout{0, 2}
                }
            };
        } else if(rotation == 3) {
            //  T
            // TT
            //  T
            return Placement {
                Placement::Bound{2, 3},
                {
                    Placement::SinglePieceLayout{1, 0},
                    Placement::SinglePieceLayout{0, 1},
                    Placement::SinglePieceLayout{1, 1},
                    Placement::SinglePieceLayout{1, 2}
                }
            };
        } else {
            PROTOCOL_VIOLATION("Invalid rotation for Shape T");
        }
    } else if (s == Shape::J) {
        if (rotation == 0) {
            //  J
            //  J
            // JJ
            return Placement {
                Placement::Bound{2, 3},
                {
                    Placement::SinglePieceLayout{1, 0},
                    Placement::SinglePieceLayout{1, 1},
                    Placement::SinglePieceLayout{0, 2},
                    Placement::SinglePieceLayout{1, 2}
                }
            };
        } else if(rotation == 1) {
            // J
            // JJJ
            return Placement {
                Placement::Bound{3, 2},
                {
                    Placement::SinglePieceLayout{0, 0},
                    Placement::SinglePieceLayout{0, 1},
                    Placement::SinglePieceLayout{1, 1},
                    Placement::SinglePieceLayout{2, 1}
                }
            };
        } else if(rotation == 2) {
            // JJ
            // J
            // J
            return Placement {
                Placement::Bound{2, 3},
                {
                    Placement::SinglePieceLayout{0, 0},
                    Placement::SinglePieceLayout{1, 0},
                    Placement::SinglePieceLayout{0, 1},
                    Placement::SinglePieceLayout{0, 2}
                }
            };
        } else if(rotation == 3) {
            // JJJ
            //   J
            return Placement {
                Placement::Bound{3, 2},
                {
                    Placement::SinglePieceLayout{0, 0},
                    Placement::SinglePieceLayout{1, 0},
                    Placement::SinglePieceLayout{2, 0},
                    Placement::SinglePieceLayout{2, 1}
                }
            };
        } else {
            PROTOCOL_VIOLATION("Invalid rotation for Shape J");
        }
    } else if (s == Shape::L) {
        if (rotation == 0) {
            // L
            // L
            // LL
            return Placement {
                Placement::Bound{2, 3},
                {
                    Placement::SinglePieceLayout{0, 0},
                    Placement::SinglePieceLayout{0, 1},
                    Placement::SinglePieceLayout{0, 2},
                    Placement::SinglePieceLayout{1, 2}
                }
            };
        } else if(rotation == 1) {
            // LLL
            // L
            return Placement {
                Placement::Bound{3, 2},
                {
                    Placement::SinglePieceLayout{0, 0},
                    Placement::SinglePieceLayout{1, 0},
                    Placement::SinglePieceLayout{2, 0},
                    Placement::SinglePieceLayout{0, 1}
                }
            };
        } else if(rotation == 2) {
            // LL
            //  L
            //  L
            return Placement {
                Placement::Bound{2, 3},
                {
                    Placement::SinglePieceLayout{0, 0},
                    Placement::SinglePieceLayout{1, 0},
                    Placement::SinglePieceLayout{1, 1},
                    Placement::SinglePieceLayout{1, 2}
                }
            };
        } else if(rotation == 3) {
            //   L
            // LLL
            return Placement {
                Placement::Bound{3, 2},
                {
                    Placement::SinglePieceLayout{2, 0},
                    Placement::SinglePieceLayout{0, 1},
                    Placement::SinglePieceLayout{1, 1},
                    Placement::SinglePieceLayout{2, 1}
                }
            };
        } else {
            PROTOCOL_VIOLATION("Invalid rotation for Shape L");
        }
    } else if (s == Shape::S) {
        if (rotation == 0) {
            //  SS
            // SS
            return Placement {
                Placement::Bound{3, 2},
                {
                    Placement::SinglePieceLayout{1, 0},
                    Placement::SinglePieceLayout{2, 0},
                    Placement::SinglePieceLayout{0, 1},
                    Placement::SinglePieceLayout{1, 1}
                }
            };
        } else if(rotation == 1) {
            // S
            // SS
            //  S
            return Placement {
                Placement::Bound{2, 3},
                {
                    Placement::SinglePieceLayout{0, 0},
                    Placement::SinglePieceLayout{0, 1},
                    Placement::SinglePieceLayout{1, 1},
                    Placement::SinglePieceLayout{1, 2}
                }
            };
        } else {
            PROTOCOL_VIOLATION("Invalid rotation for Shape S");
        }
    } else if (s == Shape::Z) {
        if (rotation == 0) {
            // ZZ
            //  ZZ
            return Placement {
                Placement::Bound{3, 2},
                {
                    Placement::SinglePieceLayout{0, 0},
                    Placement::SinglePieceLayout{1, 0},
                    Placement::SinglePieceLayout{1, 1},
                    Placement::SinglePieceLayout{2, 1}
                }
            };
        } else if(rotation == 1) {
            //  Z
            // ZZ
            // Z
            return Placement {
                Placement::Bound{2, 3},
                {
                    Placement::SinglePieceLayout{1, 0},
                    Placement::SinglePieceLayout{0, 1},
                    Placement::SinglePieceLayout{1, 1},
                    Placement::SinglePieceLayout{0, 2}
                }
            };
        } else {
            PROTOCOL_VIOLATION("Invalid rotation for Shape Z");
        }
    }
    PROTOCOL_VIOLATION("Invalid tetromino shape");
}

}
}
