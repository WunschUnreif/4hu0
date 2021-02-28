#include "GameTree.hpp"

#include <exception>
#include <random>
#include <ctime>
#include <iostream>

namespace cc4huo {
namespace mcts {

GameTreeEdge::GameTreeEdge(std::weak_ptr<GameTreeNode> from, const mech::ChessMove & move): move(move) {
    child = std::make_shared<GameTreeNode>(from.lock()->configuration.configuration_after_legal_move(move), from);
}

void GameTreeNode::back_propagate(bool winned) {
    std::weak_ptr<GameTreeNode> node = shared_from_this();

    while(node.lock()->parent.lock() != node.lock()) {
        if(winned) {
            node.lock()->win_count++;
        }
        node.lock()->access_count++;

        node = node.lock()->parent.lock();
    }
    
    if(winned) {
        node.lock()->win_count++;
    }
    node.lock()->access_count++;
}

void GameTreeNode::expand() {
    static std::default_random_engine engine;
    static bool seeded = false;

    if(seeded == false) {
        engine.seed(time(NULL));
        seeded = true;
    }

    std::uniform_int_distribution<int> distrb(0, legal_moves.size() - 1);
    auto choice = distrb(engine);

    auto edge = GameTreeEdge(shared_from_this(), legal_moves[choice]);
    edges.push_back(edge);
    legal_moves.erase(legal_moves.begin() + choice);

    auto child = edge.child;
    
    child->simulate();
}

void GameTreeNode::simulate() {
    static std::default_random_engine rand_engine;
    static bool seeded = false;

    if(seeded == false) {
        rand_engine.seed(time(NULL));
        seeded = true;
    }

    auto config = configuration;

    while(true) {
        auto move_list = config.legal_move_list();

        if( move_list.size() == 0 || 
            config.board.attackable_chessman_count() == 0 ||
            config.neutral_steps >= 60
        ) break;

        std::uniform_int_distribution<int> distrb(0, move_list.size() - 1);
        auto choice = move_list[distrb(rand_engine)];

        config.commit_legal_move(choice);
    }

    auto status = config.status();

    if((status == mech::RED_WIN && root_party == mech::RED) ||
        (status == mech::BLACK_WIN && root_party == mech::BLACK)
    ) {
        back_propagate(true);
    } else {
        back_propagate(false);
    }
}

std::shared_ptr<GameTreeNode> GameTreeNode::select(TreePolicy policy) {
    return policy(shared_from_this());
}

int GameTreeNode::height() const {
    int max_height = 0;
    for(auto & e: edges) {
        max_height = std::max(max_height, e.child->height());
    }
    return max_height + 1;
}

}
}
