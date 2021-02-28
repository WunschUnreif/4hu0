#include "UCBPolicy.hpp"

#include <cmath>
#include <exception>
#include <iostream>

namespace cc4huo {
namespace mcts {

double calculate_ucb(std::shared_ptr<GameTreeNode> node, double C) {
    auto win_freq = static_cast<double>(node->win_count) / static_cast<double>(node->access_count == 0 ? 1 : node->access_count);

    if(node->parent.lock() == node) {
        return win_freq;
    } else {
        auto ucb = win_freq + C * sqrt(
            2.0 * log(node->parent.lock()->access_count) 
                / static_cast<double>(node->access_count == 0 ? 1 : node->access_count)
        );

        // std::cout << ucb << ' ' << node->parent.lock()->access_count << std::endl;

        return ucb;
    }
}

std::shared_ptr<GameTreeNode> choose_node(std::shared_ptr<GameTreeNode> from, double C) {
    if(from->legal_moves.size() > 0) {
        return from;
    }

    // if(from->configuration.status() != mech::ACTIVE) {
    //     throw std::runtime_error("[choose_node]: error");
    // }

    std::shared_ptr<GameTreeNode> choice = nullptr;
    double max_ucb = 0;
    
    for(auto & edge : from->edges) {
        if(edge.child->configuration.status() == mech::ACTIVE) {
            auto ucb = calculate_ucb(edge.child, C);
            if(ucb > max_ucb) {
                max_ucb = ucb;
                choice = edge.child;
            }
        }
    }

    return choose_node(choice, C);
}

GameTreeNode::TreePolicy gen_ucb_policy(double C) {
    return [&](std::shared_ptr<GameTreeNode> node) {
        return choose_node(node, C);
    };
}

}
}
