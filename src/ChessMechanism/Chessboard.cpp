#include <cctype>
#include <exception>

#include "Chessboard.hpp"

namespace cc4huo {
namespace mech {

MaybeChessman& Chessboard::operator[] (ChessmanPosition pos) {
    return board[pos.x][pos.y];
}

MaybeChessman* Chessboard::operator[] (int x) {
    return board[x];
}

std::string Chessboard::to_fen() const {
    std::string fen = "";
    int blank_count = 0;

    for(int y = 9; y >= 0; --y) {
        for(int x = 0; x <= 8; ++x) {
            auto piece = board[x][y];

            if(piece.is_chessman == false) {
                blank_count++;
            } else {
                if(blank_count != 0) {
                    fen += std::to_string(blank_count);
                    blank_count = 0;
                }
                fen += piece.real_chessman.chessman_name();
            }
        }

        if(blank_count != 0) {
            fen += std::to_string(blank_count);
            blank_count = 0;
        }

        if(y != 0) {
            fen += "/";
        }
    }

    return fen;
}

Chessboard Chessboard::from_fen(const std::string & fen) {
    Chessboard result;
    int fen_read_pointer = 0;

    for(int y = 9; y >= 0; --y) {
        for(int x = 0; x <= 8; ++x) {
            if(fen_read_pointer >= fen.length()) {
                throw std::runtime_error("Invalid fen string");
            }

            auto fen_char = fen[fen_read_pointer++];

            if(isnumber(fen_char)) {
                x += fen_char - '0' - 1;
            } else {
                Chessman piece(fen_char);
                result[x][y] = piece;
            }
        }

        if(y != 0 && fen_read_pointer < fen.length() && fen[fen_read_pointer++] != '/') {
            throw std::runtime_error("Invalid fen string");
        }
    }

    return result;
}

}
}
