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
            log(static_cast<double>(node->parent.lock()->access_count)) 
            / static_cast<double>(node->access_count == 0 ? 1 : node->access_count)
        );

        // std::cout << ucb << ' ' << C << ' ' << node->parent.lock()->access_count << ' ' << node->access_count << std::endl;

        return ucb;
    }
}

std::shared_ptr<GameTreeNode> choose_node(std::shared_ptr<GameTreeNode> from, double C) {
    // std::cout << "from = " << from.get() << std::endl;

    if(from->legal_moves.size() > 0 || from->edges.size() == 0) {
        return from;
    }

    if(from->is_always_win) {
        return from;
    }

    // if(from->configuration.status() != mech::ACTIVE) {
    //     throw std::runtime_error("[choose_node]: error");
    // }

    std::shared_ptr<GameTreeNode> choice = nullptr;
    double max_ucb = -100;

    // std::cout << "choosing in " << from->edges.size() << std::endl;

    // std::cout << "ucb's: " << "[" << C << "] ";
    
    for(auto & edge : from->edges) {
        if(edge.child->is_always_win == false) {
            auto ucb = calculate_ucb(edge.child, C);
            // std::cout << edge.move.move_name() << "=" << ucb << ' ';
            if(ucb > max_ucb) {
                max_ucb = ucb;
                choice = edge.child;
            }
        }
    }

    // std::cout << std::endl;

    if(choice == nullptr) {
        std::cout << "[choose_node] Cannot choose!" << std::endl;
        return from;
    }

    return choose_node(choice, C);
}

GameTreeNode::TreePolicy gen_ucb_policy(double C) {
    return [C](std::shared_ptr<GameTreeNode> node) {
        // std::cout << C << std::endl;
        return choose_node(node, C);
    };
}

}
}
