#ifndef AGENT_HPP
#define AGENT_HPP

#include <queue>

#include "ChessMechanism.hpp"
#include "SelfPlayData.hpp"

namespace cc4huo {
namespace nnmodel {

mech::ChessMove sample_from_distribution(std::deque<SelfPlayData::MoveProb> distribution);

struct Agent {

    virtual void evaluate(const mech::Configuration & config, std::deque<SelfPlayData::MoveProb> & distribution) = 0;

    std::vector<SelfPlayData> play();

};

}
}

#endif // !AGENT_HPP
