#ifndef CHESSMOVE_HPP
#define CHESSMOVE_HPP

#include "Chessman.hpp"

#include <string>

namespace cc4huo {
namespace mech {

struct ChessMove {
    ChessmanPosition from;
    ChessmanPosition to;

    ChessMove(ChessmanPosition from, ChessmanPosition to): from(from), to(to) {}

    bool operator==(const ChessMove & rhs) { return from == rhs.from && to == rhs.to; }

    /**
     * @brief get the coordinate represent of the move (e.g. 'h2h4')
     * 
     * @return std::string the representation of the move
     */
    std::string move_name() const;
};

}
}

#endif // !CHESSMOVE_HPP
