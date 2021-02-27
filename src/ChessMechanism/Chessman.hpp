#ifndef CHESSMAN_HPP
#define CHESSMAN_HPP

#include <utility>
#include <queue>
#include <string>

namespace cc4huo {
namespace mech {

enum Party {
    BLACK,
    RED
};

enum ChessmanType {
    KING,
    ADVISOR,
    ELEPHANT,
    HORSE,
    ROOK,
    CANNON,
    PAWN,
};

/**
 * @brief Position of a chessman
 */
struct ChessmanPosition {
    // horizontal position, left to right = [0, 8]
    int x;

    // vertical position, bot to top = [0, 9]
    int y;

    ChessmanPosition(int x, int y): x(x), y(y) {}

    bool valid() const;
    std::string position_name() const;

    ChessmanPosition operator+ (std::pair<int, int> d) const;
};

/**
 * @brief Chessman struct
 */
struct Chessman {
    ChessmanType type = ROOK;
    Party party = RED;

    Chessman() {}
    Chessman(ChessmanType type, Party party): type(type), party(party) {}
    Chessman(char name);

    /**
     * @brief get all valid move directions. this does not consider other chessmen on the board.
     * @return std::deque<std::pair<int, int>> a deque of moves of (dx, dy)
     */
    std::deque<std::pair<int, int>> valid_move_diffs(ChessmanPosition pos) const;

    /**
     * @brief get the name of the chessman.
     * 
     * @return char chessman name, lower case for black, upper case for red.
     */
    char chessman_name() const;
};

} // mech
} // cc4huo

#endif // !CHESSMAN_HPP
