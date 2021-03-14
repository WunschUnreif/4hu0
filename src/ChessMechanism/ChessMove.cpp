#include "ChessMove.hpp"
#include <iostream>
#include <algorithm>

namespace cc4huo {
namespace mech {

ChessMove::ChessMove(const std::string & move_str): from(0, 0), to(0, 0) {
    from.x = move_str[0] - 'a';
    from.y = move_str[1] - '0';
    to.x   = move_str[2] - 'a';
    to.y   = move_str[3] - '0';
}

std::string ChessMove::move_name() const {
    return from.position_name() + to.position_name();
}

bool MoveEncoder::TABLE_INITED = false;
std::vector<ChessMove> MoveEncoder::LEGAL_MOVE_TABLES;

MoveEncoder::MoveEncoder() {
    if(TABLE_INITED) {
        return;
    }

    // Generate moves for rook, cannon, pawn
    for(int x = 0; x < 9; ++x) {
        for(int y = 0; y < 10; ++y) {
            std::deque<std::pair<int, int>> diffs;
            Chessman('r').valid_move_diffs(ChessmanPosition(x, y), diffs);
            for(auto & d : diffs) {
                LEGAL_MOVE_TABLES.emplace_back(ChessmanPosition(x, y), ChessmanPosition(x, y) + d);
            }
        }
    }

    // Generate moves for horse
    for(int x = 0; x < 9; ++x) {
        for(int y = 0; y < 10; ++y) {
            std::deque<std::pair<int, int>> diffs;
            Chessman('h').valid_move_diffs(ChessmanPosition(x, y), diffs);
            for(auto & d : diffs) {
                LEGAL_MOVE_TABLES.emplace_back(ChessmanPosition(x, y), ChessmanPosition(x, y) + d);
            }
        }
    }

    // Generate moves for elephant
    for(int x = 0; x < 9; ++x) {
        for(int y = 0; y < 10; ++y) {
            if(valid_elephant_target(ChessmanPosition(x, y), RED)) {
                std::deque<std::pair<int, int>> diffs;
                Chessman('E').valid_move_diffs(ChessmanPosition(x, y), diffs);
                for(auto & d : diffs) {
                    LEGAL_MOVE_TABLES.emplace_back(ChessmanPosition(x, y), ChessmanPosition(x, y) + d);
                }
            } else if(valid_elephant_target(ChessmanPosition(x, y), BLACK)) {
                std::deque<std::pair<int, int>> diffs;
                Chessman('e').valid_move_diffs(ChessmanPosition(x, y), diffs);
                for(auto & d : diffs) {
                    LEGAL_MOVE_TABLES.emplace_back(ChessmanPosition(x, y), ChessmanPosition(x, y) + d);
                }
            }
        }
    }

    // Generate moves for advisor
    for(int x = 0; x < 9; ++x) {
        for(int y = 0; y < 10; ++y) {
            if(valid_advisor_target(ChessmanPosition(x, y), RED)) {
                std::deque<std::pair<int, int>> diffs;
                Chessman('A').valid_move_diffs(ChessmanPosition(x, y), diffs);
                for(auto & d : diffs) {
                    LEGAL_MOVE_TABLES.emplace_back(ChessmanPosition(x, y), ChessmanPosition(x, y) + d);
                }
            } else if(valid_advisor_target(ChessmanPosition(x, y), BLACK)) {
                std::deque<std::pair<int, int>> diffs;
                Chessman('a').valid_move_diffs(ChessmanPosition(x, y), diffs);
                for(auto & d : diffs) {
                    LEGAL_MOVE_TABLES.emplace_back(ChessmanPosition(x, y), ChessmanPosition(x, y) + d);
                }
            }
        }
    }

    // Generate moves for king
    for(int x = 0; x < 9; ++x) {
        for(int y = 0; y < 10; ++y) {
            if(valid_king_target(ChessmanPosition(x, y), RED)) {
                std::deque<std::pair<int, int>> diffs;
                Chessman('K').valid_move_diffs(ChessmanPosition(x, y), diffs);
                for(auto & d : diffs) {
                    LEGAL_MOVE_TABLES.emplace_back(ChessmanPosition(x, y), ChessmanPosition(x, y) + d);
                }
            } else if(valid_king_target(ChessmanPosition(x, y), BLACK)) {
                std::deque<std::pair<int, int>> diffs;
                Chessman('k').valid_move_diffs(ChessmanPosition(x, y), diffs);
                for(auto & d : diffs) {
                    LEGAL_MOVE_TABLES.emplace_back(ChessmanPosition(x, y), ChessmanPosition(x, y) + d);
                }
            }
        }
    }

    TABLE_INITED = true;
}

int MoveEncoder::encode(const ChessMove & move) {
    int code = 0;
    // std::cout << move.move_name() << LEGAL_MOVE_TABLES.size() << std::endl;
    for(const auto & m : LEGAL_MOVE_TABLES) {
        if(m == move) {
            return code;
        }
        ++code;
    }
    return -1;
}

ChessMove MoveEncoder::decode(int code) {
    if(code >= 0 && code < LEGAL_MOVE_TABLES.size()) {
        return LEGAL_MOVE_TABLES[code];
    }
    throw std::runtime_error("[MoveEncoder::decode] Illegal move code.");
}

}
}
