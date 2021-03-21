#include "AgentPureMCTS.hpp"
#include "MCTS.hpp"
#include "ChessMechanism.hpp"


namespace cc4huo {
namespace nnmodel {

void AgentPureMCTS::evaluate(const mech::Configuration & config, std::vector<SelfPlayData::MoveProb> & distribution) {
    static mcts::GameTreeNode::TreePolicy policy = mcts::gen_ucb_policy(1.414);
    static mech::MoveEncoder encoder;

    auto game_tree = std::make_shared<mcts::GameTreeNode>(config);
    game_tree->parent = game_tree;

    if(game_tree->legal_moves.size() == 1) {
        distribution.push_back(std::make_pair(encoder.encode(game_tree->legal_moves[0]), 1.0f));
        return;
    }

    for(int _ = 0; _ < 1600; ++_) {
        auto selection = game_tree->select(policy);
        selection->expand();
    }

    int always_win_cnt = 0;
    for(auto & e : game_tree->edges) {
        if(e.child->is_always_win) {
            always_win_cnt++;
            distribution.push_back(std::make_pair(encoder.encode(e.move), 1));
        }
    }

    if(always_win_cnt > 0) {
        for(auto & moveprob : distribution) {
            moveprob.second = 1.0 / always_win_cnt;
        }
        return;
    }

    float N = game_tree->access_count;
    for(auto & e : game_tree->edges) {
        distribution.push_back(std::make_pair(
            encoder.encode(e.move),
            e.child->access_count / N
        ));
    }
}

}
}
