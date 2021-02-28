#include "Chessboard.hpp"
#include "Chessman.hpp"
#include "ChessMove.hpp"

#include <iostream>

using namespace cc4huo::mech;

int main() {
    std::cout << "Unit test [mech - 1]" << std::endl;

    std::cout << "Chessboard FEN representation test" << std::endl;

    Chessboard board = Chessboard::from_fen("rheakaehr/8c/1c7/p1p1C1p1p/9/4C4/P1P1P1P1P/9/9/RHEAKAEHR");
    std::cout << "board loaded" << std::endl;
    board.debug_print(std::cout);
    std::cout << "[1]: " << board.to_fen() << std::endl;

    std::cout << "\nMove list generator" << std::endl;
    auto moves = board.legal_move_list(RED);
    std::cout << "RED:" << std::endl;
    for(auto & m : moves) {
        std::cout << "------------------" << std::endl;
        std::cout << m.move_name() << std::endl;
        board.board_after_move(m).debug_print(std::cout);
        std::cout << "------------------" << std::endl;
    }
    
    moves = board.legal_move_list(BLACK);
    std::cout << "\nBLACK:" << std::endl;
    for(auto & m : moves) {
        std::cout << "------------------" << std::endl;
        std::cout << m.move_name() << std::endl;
        board.board_after_move(m).debug_print(std::cout);
        std::cout << "------------------" << std::endl;
    }

    return 0;
}