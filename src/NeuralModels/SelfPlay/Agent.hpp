#ifndef AGENT_HPP
#define AGENT_HPP

#include <vector>

#include "ChessMechanism.hpp"
#include "SelfPlayData.hpp"

namespace cc4huo {
namespace nnmodel {

struct Agent {

    virtual void evaluate(const mech::Configuration & config, std::vector<SelfPlayData::MoveProb> & distribution) = 0;

    std::vector<SelfPlayData> play();

};

}
}

#endif // !AGENT_HPP
