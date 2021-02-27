#ifndef CHESSBOARD_HPP
#define CHESSBOARD_HPP

#include "Chessman.hpp"
#include <string.h>

namespace cc4huo {
namespace mech {

struct MaybeChessman {
    bool is_chessman;
    Chessman real_chessman;

    MaybeChessman(): is_chessman(false) {}
    MaybeChessman(Chessman m): is_chessman(true), real_chessman(m) {}

    static MaybeChessman None()             { return MaybeChessman(); }
    static MaybeChessman Some(Chessman m)   { return MaybeChessman(m); }
};

struct Chessboard {
    /**
     * @brief The board stored by columns from left to right, each col from bottom to top.
     */
    MaybeChessman board[9][10];
    //                  ^   ^
    //                  x   y

    Chessboard() { for(int x = 0; x < 9; ++x) for(int y = 0; y < 10; ++y) board[x][y] = MaybeChessman::None(); }

    MaybeChessman& operator[] (ChessmanPosition pos);
    MaybeChessman* operator[] (int x);

    /**
     * @brief generate the FEN representation of the board configuration (only the first part)
     * 
     * @return std::string FEN representation, only fitst part
     */
    std::string to_fen() const;

    /**
     * @brief load board configuration from the first part of a FEN string
     * 
     * @param fen first part of the FEN string
     * @return Chessboard the corresponding board configuration
     * @throw std::runtime_error when the FEN string cannot be parsed
     */
    static Chessboard from_fen(const std::string & fen);
    
};

}
}

#endif // !CHESSBOARD_HPP
