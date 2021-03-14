#ifndef CHESSMOVE_HPP
#define CHESSMOVE_HPP

#include "Chessman.hpp"

#include <string>
#include <vector>

namespace cc4huo {
namespace mech {

struct ChessMove {
    ChessmanPosition from;
    ChessmanPosition to;

    ChessMove(ChessmanPosition from, ChessmanPosition to): from(from), to(to) {}
    ChessMove(const std::string & move_str);

    bool operator==(const ChessMove & rhs) const { return from == rhs.from && to == rhs.to; }

    /**
     * @brief get the coordinate represent of the move (e.g. 'h2h4')
     * 
     * @return std::string the representation of the move
     */
    std::string move_name() const;
};

struct MoveEncoder {
    static std::vector<ChessMove> LEGAL_MOVE_TABLES;
    static bool TABLE_INITED;

    MoveEncoder();

    /**
     * @brief Encode a move to an integer
     * 
     * @param move the move to encode 
     * @return int the code for the given move. -1 for illegal moves
     */
    int encode(const ChessMove & move);

    /**
     * @brief decode a move code
     * 
     * @param code 
     * @return ChessMove 
     */
    ChessMove decode(int code);
};

}
}

#endif // !CHESSMOVE_HPP
