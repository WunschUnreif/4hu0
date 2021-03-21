#ifndef GAME_TREE_HPP
#define GAME_TREE_HPP

#include "ChessMechanism.hpp"

#include <deque>
#include <memory>
#include <iostream>
#include <functional>

namespace cc4huo {
namespace mcts {

void shuffle_moves(std::deque<mech::ChessMove> & legal_moves);

struct GameTreeNode;

struct GameTreeEdge {
    mech::ChessMove move;
    std::shared_ptr<GameTreeNode> child;

    GameTreeEdge(std::shared_ptr<GameTreeNode> from, const mech::ChessMove & move);
};

struct GameTreeNode: public std::enable_shared_from_this<GameTreeNode> {
    mech::Configuration configuration;
    int access_count;
    int win_count;
    std::weak_ptr<GameTreeNode> parent;
    std::deque<GameTreeEdge> edges;
    mech::Party root_party;
    std::deque<mech::ChessMove> legal_moves;
    bool is_always_win = false;
    int always_win_child_count = 0;

    using TreePolicy = std::function<std::shared_ptr<GameTreeNode>(std::shared_ptr<GameTreeNode>)>;

    GameTreeNode(const mech::Configuration & config)
        : configuration(config)
        , access_count(0)
        , win_count(0)
        , parent()
        , root_party(config.active_party)
        , legal_moves(config.legal_move_list()) {
            shuffle_moves(legal_moves);
            auto status = configuration.status();
            if((status == mech::RED_WIN && root_party == mech::RED) ||
                (status == mech::BLACK_WIN && root_party == mech::BLACK)
            ) {
                is_always_win = true;
            }
        }

    GameTreeNode(const mech::Configuration & config, std::weak_ptr<GameTreeNode> parent)
        : configuration(config)
        , access_count(0)
        , win_count(0)
        , parent(parent)
        , root_party(parent.lock()->root_party)
        , legal_moves(config.legal_move_list()) {
            shuffle_moves(legal_moves);
            auto status = configuration.status();
            if((status == mech::RED_WIN && root_party == mech::RED) ||
                (status == mech::BLACK_WIN && root_party == mech::BLACK)
            ) {
                is_always_win = true;
                parent.lock()->notify_always_win();
            }
        }

    /**
     * @brief Propagate the winning info back to root
     * 
     * @param winned the simulation result is winning or not
     */
    void back_propagate(bool winned);

    /**
     * @brief Notify the current node that it has an always-winning child node.
     */
    void notify_always_win();

    /**
     * @brief expand a child to this node, will do simulate
     */
    void expand();

    /**
     * @brief simulate by random moves until end game. will do bp
     */
    void simulate();

    /**
     * @brief select a node to explore using a given policy
     * 
     * @param policy a function takes a node and return the node it chose
     * @return std::shared_ptr<GameTreeNode> the chosen node
     */
    std::shared_ptr<GameTreeNode> select(TreePolicy policy);

    /**
     * @brief Get the tree height under this node.
     * 
     * @return int tree height
     */
    int height() const;

    /**
     * @brief Get the node count in this subtree.
     * 
     * @return int node number.
     */
    int node_count() const;
};

}
}

#endif // !GAME_TREE_HPP
