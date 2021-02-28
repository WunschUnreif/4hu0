#include "Game.hpp"

#include <exception>
#include <sstream>

namespace cc4huo {
namespace mech {

Game Game::new_game() {
    Game game;
    game.history_configurations.push_back(Configuration::init_config());
    return game;
}

Game Game::new_game_from_fen(const std::string & fen) {
    Game game;
    game.history_configurations.push_back(Configuration::from_fen(fen));
    return game;
}

const Configuration & Game::current_configuration() const {
    return history_configurations.back();
}

void Game::commit_legal_move(const ChessMove & move) {
    history_moves.push_back(move);
    history_configurations.push_back(current_configuration().configuration_after_legal_move(move));
}

void Game::revert() {
    if(history_moves.empty()) {
        throw std::runtime_error("[Game::revert] no moves has been made");
    }

    history_moves.pop_back();
    history_configurations.pop_back();
}

bool Game::is_ended() {
    return current_configuration().status() != ConfigurationStatus::ACTIVE;
}

std::string Game::game_record(const std::string & event_name) const {
    std::ostringstream record_stream;
    record_stream << "[Event \"" << event_name << "\"]" << std::endl;
    record_stream << "[Site \"wunschunreif.local\"]" << std::endl;
    record_stream << "[Variant \"xiangqi\"]" << std::endl << std::endl;

    int round = 1;

    for(auto move = history_moves.begin(); move != history_moves.end(); ) {
        record_stream << std::to_string(round++) << ". ";
        record_stream << move->move_name() << " ";
        move++;
        if(move != history_moves.end()) {
            record_stream << move->move_name();
            move++;
        }
        record_stream << std::endl;
    }

    record_stream << std::endl;

    switch(current_configuration().status()) {
    case ConfigurationStatus::ACTIVE:    record_stream << "*" << std::endl; break;
    case ConfigurationStatus::DRAW:      record_stream << "1/2-1/2" << std::endl; break;
    case ConfigurationStatus::RED_WIN:   record_stream << "1-0" << std::endl; break;
    case ConfigurationStatus::BLACK_WIN: record_stream << "0-1" << std::endl; break;
    }

    return record_stream.str();
}

}
}
