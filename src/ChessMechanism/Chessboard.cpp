#include <cctype>
#include <exception>
#include <iostream>
#include <cmath>

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

void Chessboard::debug_print(std::ostream & out) const {
    for(int y = 9; y >= 0; --y) {
        for(int x = 0; x <= 8; ++x) {
            if(board[x][y].is_chessman) {
                std::cout << board[x][y].real_chessman.chessman_name() << ' ';
            } else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
}

std::deque<ChessMove> Chessboard::checkless_move_list(Party party) const {
    std::deque<ChessMove> result;

    for(int x = 0; x < 9; ++x) {
        for(int y = 0; y < 10; ++y) {
            const auto & piece = board[x][y];
            if(piece.is_chessman && piece.real_chessman.party == party) {
                const auto && list = checkless_move_list_for(ChessmanPosition(x, y));
                result.insert(result.end(), list.begin(), list.end());
            }
        }
    }

    return result;
}

std::deque<ChessMove> Chessboard::checkless_move_list_for(ChessmanPosition pos) const {
    std::deque<ChessMove> result;
    const auto & piece = board[pos.x][pos.y];

    if(piece.is_chessman == false) {
        return result;
    }

    auto diffs = piece.real_chessman.valid_move_diffs(pos);
    for(auto & d : diffs) {
        result.push_back(ChessMove(pos, pos + d));
    }

    checkless_filter_move_list(result, piece.real_chessman.type);

    return result;
}

void Chessboard::checkless_filter_move_list(std::deque<ChessMove> & list, ChessmanType type) const {
    auto current = list.begin();

    while(current != list.end()) {
        bool valid = false;
        switch(type) {
        case ROOK       : valid = checkless_valid_move_for_rook(*current); break;
        case HORSE      : valid = checkless_valid_move_for_horse(*current); break;
        case ELEPHANT   : valid = checkless_valid_move_for_elephant(*current); break;
        case ADVISOR    : valid = checkless_valid_move_for_advisor(*current); break;
        case KING       : valid = checkless_valid_move_for_king(*current); break;
        case CANNON     : valid = checkless_valid_move_for_cannon(*current); break;
        case PAWN       : valid = checkless_valid_move_for_pawn(*current); break;

        default: throw std::runtime_error("[Chessboard::checkless_filter_move_list] invalid chessman kind");
        }

        if(valid == false) {
            current = list.erase(current);
        } else {
            ++current;
        }
    }
}

bool Chessboard::checkless_valid_move_for_rook(const ChessMove & move) const {
    int dx = move.to.x - move.from.x;
    int dy = move.to.y - move.from.y;

    dx = dx > 0 ? 1 : (dx < 0 ? -1 : 0);
    dy = dy > 0 ? 1 : (dy < 0 ? -1 : 0);

    int middle_piece_count = 0;
    auto pos = move.from + std::make_pair(dx, dy);

    while(pos != move.to) {
        middle_piece_count += board[pos.x][pos.y].is_chessman ? 1 : 0;
        pos = pos + std::make_pair(dx, dy);
    }

    if(middle_piece_count != 0) return false;

    const auto party = board[move.from.x][move.from.y].real_chessman.party;
    const auto & target = board[move.to.x][move.to.y];

    // Cannot take a piece from self party
    if(target.is_chessman && target.real_chessman.party == party) return false;

    return true;
}

bool Chessboard::checkless_valid_move_for_cannon(const ChessMove & move) const {
    int dx = move.to.x - move.from.x;
    int dy = move.to.y - move.from.y;

    dx = dx > 0 ? 1 : (dx < 0 ? -1 : 0);
    dy = dy > 0 ? 1 : (dy < 0 ? -1 : 0);

    const auto party = board[move.from.x][move.from.y].real_chessman.party;
    const auto & target = board[move.to.x][move.to.y];

    // Cannot take a piece from self party
    if(target.is_chessman && target.real_chessman.party == party) return false;

    // count middle pieces
    int middle_piece_count = 0;
    auto pos = move.from + std::make_pair(dx, dy);

    while(pos != move.to) {
        middle_piece_count += board[pos.x][pos.y].is_chessman ? 1 : 0;
        pos = pos + std::make_pair(dx, dy);
    }

    // cannon takes piece across exactly one piece
    if(target.is_chessman) {
        return middle_piece_count == 1;
    } else {
        return middle_piece_count == 0;
    }
}

bool Chessboard::checkless_valid_move_for_pawn(const ChessMove & move) const {
    const auto party = board[move.from.x][move.from.y].real_chessman.party;
    const auto & target = board[move.to.x][move.to.y];

    // Cannot take a piece from self party
    if(target.is_chessman && target.real_chessman.party == party) return false;
    else return true;
}

bool Chessboard::checkless_valid_move_for_advisor(const ChessMove & move) const {
    const auto party = board[move.from.x][move.from.y].real_chessman.party;
    const auto & target = board[move.to.x][move.to.y];

    // Cannot take a piece from self party
    if(target.is_chessman && target.real_chessman.party == party) return false;
    else return true;
}

bool Chessboard::checkless_valid_move_for_king(const ChessMove & move) const {
    const auto party = board[move.from.x][move.from.y].real_chessman.party;
    const auto & target = board[move.to.x][move.to.y];

    // Cannot take a piece from self party
    if(target.is_chessman && target.real_chessman.party == party) return false;
    else return true;
}

bool Chessboard::checkless_valid_move_for_elephant(const ChessMove & move) const {
    const auto party = board[move.from.x][move.from.y].real_chessman.party;
    const auto & target = board[move.to.x][move.to.y];

    // Cannot take a piece from self party
    if(target.is_chessman && target.real_chessman.party == party) return false;
    
    int dx = (move.to.x - move.from.x) / 2;
    int dy = (move.to.y - move.from.y) / 2;
    const auto & eye = board[move.from.x + dx][move.from.y + dy];

    return eye.is_chessman == false;
}

bool Chessboard::checkless_valid_move_for_horse(const ChessMove & move) const {
    const auto party = board[move.from.x][move.from.y].real_chessman.party;
    const auto & target = board[move.to.x][move.to.y];

    // Cannot take a piece from self party
    if(target.is_chessman && target.real_chessman.party == party) return false;
    
    int dx = (move.to.x - move.from.x);
    int dy = (move.to.y - move.from.y);

    dx = abs(dx) == 1 ? 0 : dx / 2;
    dy = abs(dy) == 1 ? 0 : dy / 2;

    const auto & leg = board[move.from.x + dx][move.from.y + dy];

    return leg.is_chessman == false;
}

Chessboard Chessboard::board_after_move(const ChessMove & move) const {
    Chessboard result = *this;
    result[move.to] = result[move.from];
    result[move.from] = MaybeChessman::None();
    return result;
}

bool Chessboard::is_been_checked(Party party) const {
    auto king_pos = king_position_of(party);
    auto opponent_party = opponent_party_of(party);
    auto opponent_king_pos = king_position_of(opponent_party);
    auto opponent_moves = checkless_move_list(opponent_party);

    // no king of this party found.
    if(king_pos.x < 0) {
        return true;
    }

    // no king of opponent party found: already win.
    if(opponent_king_pos.x < 0) {
        return false;
    }
    
    // king can be taken by opponent
    for(const auto & m: opponent_moves) {
        if(m.to == king_pos) {
            return true;
        }
    }

    // king face to face directly
    if(king_pos.x == opponent_king_pos.x) {
        int dy = opponent_king_pos.y - king_pos.y;
        dy = dy / abs(dy);

        int middle_piece_count = 0;
        for(int y = king_pos.y + dy; y != opponent_king_pos.y; y += dy) {
            middle_piece_count += board[king_pos.x][y].is_chessman ? 1 : 0;
        }

        if(middle_piece_count == 0) return true;
    }

    return false;
}

ChessmanPosition Chessboard::king_position_of(Party party) const {
    if(party == BLACK) {
        for(int x = 3; x <= 5; ++x) {
            for(int y = 7; y <= 9; ++y) {
                if(board[x][y].is_chessman && board[x][y].real_chessman.type == KING) {
                    return ChessmanPosition(x, y);
                }
            }
        }
    } else {
        for(int x = 3; x <= 5; ++x) {
            for(int y = 0; y <= 2; ++y) {
                if(board[x][y].is_chessman && board[x][y].real_chessman.type == KING) {
                    return ChessmanPosition(x, y);
                }
            }
        }
    }

    return ChessmanPosition(-1, -1);
}

std::deque<ChessMove> Chessboard::legal_move_list(Party party) const {
    auto checkless_moves = checkless_move_list(party);
    auto current = checkless_moves.begin();

    while(current != checkless_moves.end()) {
        if(this->board_after_move(*current).is_been_checked(party)) {
            current = checkless_moves.erase(current);
        } else {
            ++current;
        }
    }

    return checkless_moves;
}

}
}
