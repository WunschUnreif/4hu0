#ifndef UCB_POLICY_HPP
#define UCB_POLICY_HPP

#include "GameTree.hpp"
#include "ChessMechanism.hpp"

namespace cc4huo {
namespace mcts {

GameTreeNode::TreePolicy gen_ucb_policy(double C);

}
}

#endif // !UCB_POLICY_HPP
