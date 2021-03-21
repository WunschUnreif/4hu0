#ifndef SELF_PLAY_DATA_HPP
#define SELF_PLAY_DATA_HPP

#include <utility>
#include <vector>

#include "ChessMechanism.hpp"

namespace cc4huo {
namespace nnmodel {

struct SelfPlayData {
    using MoveProb = std::pair<int, float>; // Pr[ first <as move code> ] = second <as probability>

    mech::Configuration config = mech::Configuration::init_config();
    std::vector<MoveProb> distribution;

    /// 1 = red win, -1 = black win, 0 = tie
    int value;

    std::string to_string() const;
    static SelfPlayData from_string(const std::string & line1, const std::string & line2, const std::string & line3);
};

}
}

#endif // !SELF_PLAY_DATA_HPP
