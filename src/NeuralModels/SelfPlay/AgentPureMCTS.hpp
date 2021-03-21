#ifndef AGENT_PURE_MCTS_HPP
#define AGENT_PURE_MCTS_HPP

#include "Agent.hpp"
#include "SelfPlayData.hpp"

namespace cc4huo {
namespace nnmodel {

struct AgentPureMCTS : public Agent {
    virtual void evaluate(const mech::Configuration & config, std::vector<SelfPlayData::MoveProb> & distribution) override;
};

}
}

#endif // !AGENT_PURE_MCTS_HPP
