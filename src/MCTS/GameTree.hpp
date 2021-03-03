#ifndef GAME_TREE_HPP
#define GAME_TREE_HPP

#include "ChessMechanism.hpp"

#include <deque>
#include <memory>
#include <functional>

namespace cc4huo {
namespace mcts {

struct GameTreeNode;

struct GameTreeEdge {
    mech::ChessMove move;
    std::shared_ptr<GameTreeNode> child;

    GameTreeEdge(std::weak_ptr<GameTreeNode> from, const mech::ChessMove & move);
};

struct GameTreeNode: public std::enable_shared_from_this<GameTreeNode> {
    mech::Configuration configuration;
    int access_count;
    int win_count;
    std::weak_ptr<GameTreeNode> parent;
    std::deque<GameTreeEdge> edges;
    mech::Party root_party;
    std::deque<mech::ChessMove> legal_moves;

    using TreePolicy = std::function<std::shared_ptr<GameTreeNode>(std::shared_ptr<GameTreeNode>)>;

    GameTreeNode(const mech::Configuration & config)
        : configuration(config)
        , access_count(0)
        , win_count(0)
        , parent()
        , root_party(config.active_party)
        , legal_moves(config.legal_move_list()) {}

    GameTreeNode(const mech::Configuration & config, std::weak_ptr<GameTreeNode> parent)
        : configuration(config)
        , access_count(0)
        , win_count(0)
        , parent(parent)
        , root_party(parent.lock()->root_party)
        , legal_moves(config.legal_move_list()) {}

    /**
     * @brief Propagate the winning info back to root
     * 
     * @param winned the simulation result is winning or not
     */
    void back_propagate(bool winned);

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
