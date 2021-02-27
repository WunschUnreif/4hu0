#include "Chessboard.hpp"
#include "Chessman.hpp"

#include <iostream>

using namespace cc4huo::mech;

int main() {
    std::cout << "Unit test [mech - 1]" << std::endl;

    std::cout << "Chessboard FEN representation test" << std::endl;

    Chessboard board = Chessboard::from_fen("rheakaehr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RHEAKAEHR");
    std::cout << "board loaded" << std::endl;

    for(int y = 9; y >= 0; --y) {
        for(int x = 0; x <= 8; ++x) {
            std::cout << board[x][y].is_chessman << board[x][y].real_chessman.type << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << "[1]: " << board.to_fen() << std::endl;

    return 0;
}