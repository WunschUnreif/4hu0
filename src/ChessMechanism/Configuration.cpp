#include "Configuration.hpp"

#include <cstdlib>

namespace cc4huo {
namespace mech {

Configuration Configuration::init_config() {
    return Configuration(
        Chessboard::from_fen("rheakaehr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RHEAKAEHR"),
        RED,
        0,
        0
    );
}

ConfigurationStatus Configuration::status() const {
    // decide draw
    if(neutral_steps >= 60) {
        return DRAW;
    }

    int attackable_chessman_count = board.attackable_chessman_count();

    if(attackable_chessman_count == 0) {
        return DRAW;
    }

    if(active_party == RED) {
        if(board.legal_move_list(RED).size() == 0) {
            return BLACK_WIN;
        }
    } else {
        if(board.legal_move_list(BLACK).size() == 0) {
            return RED_WIN;
        }
    }

    return ACTIVE;
}

std::deque<ChessMove> Configuration::legal_move_list() const {
    return board.legal_move_list(active_party);
}

std::string Configuration::to_fen() const {
    return board.to_fen() + 
            (active_party == RED ? " w" : " b") + 
            " " + std::to_string(neutral_steps) +
            " " + std::to_string(step_count);
}

Configuration Configuration::from_fen(const std::string & fen) {
    auto first_blank = fen.find_first_of(' ');
    auto board_desc = fen.substr(0, first_blank - 0);

    auto second_blank = fen.find_first_of(' ', first_blank + 1);
    auto active_desc = fen.substr(first_blank + 1, second_blank - first_blank - 1);

    auto third_blank = fen.find_first_of(' ', second_blank + 1);
    auto neutral_desc = fen.substr(second_blank + 1, third_blank - second_blank - 1);

    auto step_count_desc = fen.substr(third_blank + 1);

    return Configuration(
        Chessboard::from_fen(board_desc),
        active_desc[0] == 'b' ? BLACK : RED,
        atoi(neutral_desc.c_str()),
        atoi(step_count_desc.c_str())
    );
}

void Configuration::commit_legal_move(const ChessMove & move) {
    if(board[move.to].is_chessman) {
        neutral_steps = 0;
    } else {
        neutral_steps++;
    }
    board.commit_move(move);
    active_party = opponent_party_of(active_party);
    step_count++;
}

Configuration Configuration::configuration_after_legal_move(const ChessMove & move) const {
    Configuration result = *this;
    result.commit_legal_move(move);
    return result;
}

}
}
