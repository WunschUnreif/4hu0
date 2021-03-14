#include "GameTree.hpp"

#include <exception>
#include <random>
#include <ctime>
#include <iostream>

namespace cc4huo {
namespace mcts {

GameTreeEdge::GameTreeEdge(std::shared_ptr<GameTreeNode> from, const mech::ChessMove & move): move(move) {
    child = std::make_shared<GameTreeNode>(from->configuration.configuration_after_legal_move(move), from);
}

void GameTreeNode::notify_always_win() {
    ++always_win_child_count;
    if(always_win_child_count == edges.size() && legal_moves.empty()) {
        is_always_win = true;
        if(parent.lock() != shared_from_this()) {
            parent.lock()->notify_always_win();
        }
    }
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
    if(legal_moves.size() == 0) {
        auto status = configuration.status();
        if((status == mech::RED_WIN && root_party == mech::RED) ||
            (status == mech::BLACK_WIN && root_party == mech::BLACK)
        ) {
            std::cout << "Impossible" << std::endl;
            back_propagate(true);
        } else {
            back_propagate(false);
        }
        return;
    }

    auto edge = GameTreeEdge(shared_from_this(), legal_moves[0]);

    edges.push_back(edge);
    legal_moves.erase(legal_moves.begin());

    auto child = edge.child;
    
    // for(int i = 0; i < 20; ++i) {
    child->simulate();
    // }
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
        std::deque<mech::ChessMove> move_list;
        config.board.checkless_move_list(config.active_party, move_list);

        if( move_list.size() == 0 || 
            config.board.attackable_chessman_count() == 0 ||
            config.board.chessman_count(mech::KING, mech::RED) == 0 ||
            config.board.chessman_count(mech::KING, mech::BLACK) == 0 ||
            config.neutral_steps >= 60
        ) break;

        mech::ChessMove choice = move_list[0];

        while(true) {
            if(move_list.size() == 0) {
                goto L_FAILED;
            }

            std::uniform_int_distribution<int> distrb(0, move_list.size() - 1);
            auto choice_it = move_list.begin() + distrb(rand_engine);
            choice = *choice_it;

            if(config.configuration_after_legal_move(choice).board.is_been_checked(config.active_party)) {
                move_list.erase(choice_it);
                continue;
            }

            break;
        }

        config.commit_legal_move(choice);
    }

L_FAILED:
    auto status = config.status();

    if(status == mech::ACTIVE) {
        std::cout << (config.active_party == mech::RED) << std::endl;
        config.board.debug_print(std::cout);
        throw std::runtime_error("[GameTreeNode::simulate] False Failure.");
    }

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

int GameTreeNode::node_count() const {
    int nodes = 1;
    for(auto & e : edges) {
        nodes += e.child->node_count();
    }
    return nodes;
}

}
}
