#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include "Chessman.hpp"
#include "Chessboard.hpp"
#include "ChessMove.hpp"

#include <queue>

namespace cc4huo {
namespace mech {

enum ConfigurationStatus {
    ACTIVE,
    DRAW,
    RED_WIN,
    BLACK_WIN
};

struct Configuration {
    Chessboard board;
    Party active_party;
    int neutral_steps;
    int step_count;

    Configuration(const Chessboard & board, Party active, int neutral, int steps)
        : board(board), active_party(active), neutral_steps(neutral), step_count(steps) {}

    /**
     * @brief Get the status of the current configuration
     * 
     * @return ConfigurationStatus status
     */
    ConfigurationStatus status() const;

    /**
     * @brief Get the legal move list of the current configuration
     * 
     * @return std::deque<ChessMove> legal move list
     */
    std::deque<ChessMove> legal_move_list() const;

    /**
     * @brief Convert to FEN representation (full)
     * 
     * @return std::string FEN representation
     */
    std::string to_fen() const;

    /**
     * @brief Commit a legal move to the current configuration. Active party will switch automatically.
     * 
     * @param move a legal move
     */
    void commit_legal_move(const ChessMove & move);

    /**
     * @brief Get the configuration after a legal move.
     * 
     * @param move a legal move
     * @return Configuration the new configuration
     */
    Configuration configuration_after_legal_move(const ChessMove & move) const;

    /**
     * @brief Load configuration from a FEN string
     * 
     * @param fen the FEN string
     * @return Configuration the corresponding configuration
     */
    static Configuration from_fen(const std::string & fen);

    /**
     * @brief Get the initial configuration for xiangqi.
     * 
     * @return Configuration the initial configuration
     */
    static Configuration init_config();
};

}
}

#endif // !CONFIGURATION_HPP
