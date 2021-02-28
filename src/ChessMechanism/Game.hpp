#ifndef GAME_HPP
#define GAME_HPP

#include "Chessboard.hpp"
#include "Chessman.hpp"
#include "ChessMove.hpp"
#include "Configuration.hpp"

#include <queue>
#include <string>

namespace cc4huo {
namespace mech {

struct Game {
    std::deque<Configuration> history_configurations;
    std::deque<ChessMove> history_moves;

    /**
     * @brief Create a new game with the default initial configuration.
     * 
     * @return Game the default initial game
     */
    static Game new_game();

    /**
     * @brief Create a new game with the given configuration's FEN representation.
     * 
     * @param fen FEN representation
     * @return Game the game start at the configuration described by the FEN
     */
    static Game new_game_from_fen(const std::string & fen);

    /**
     * @brief Get the current configuration
     * 
     * @return Configuration current configuration
     */
    const Configuration & current_configuration() const;

    /**
     * @brief Commit a legal move.
     * 
     * @param move a legal move
     * @throw std::runtime_error when no moves has been made
     */
    void commit_legal_move(const ChessMove & move);

    /**
     * @brief revert one step
     */
    void revert();

    /**
     * @brief Decide whether the game is end.
     * 
     * @return true game over 
     * @return false game can continue
     */
    bool is_ended();

    /**
     * @brief Generate the game record.
     * 
     * @return std::string game record compatible with XBoard
     */
    std::string game_record(const std::string & event_name = "") const;
};

}
}

#endif // !GAME_HPP