#include "ChessMove.hpp"

namespace cc4huo {
namespace mech {

std::string ChessMove::move_name() const {
    return from.position_name() + to.position_name();
}

}
}
