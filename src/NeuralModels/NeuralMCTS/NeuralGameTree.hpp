#ifndef NEURAL_GAME_TREE_HPP
#define NEURAL_GAME_TREE_HPP

#include <queue>
#include <memory>

#include "ChessMechanism.hpp"
#include "NeuralModel.hpp"
#include "SelfPlay/SelfPlayData.hpp"
#include "SelfPlay/AgentNeural.hpp"

namespace cc4huo {
namespace nnmodel {

struct NeuralGameTreeNode;

struct NeuralGameTreeEdge {
    std::shared_ptr<NeuralGameTreeNode> child;
    int move_code;

    NeuralGameTreeEdge(std::shared_ptr<NeuralGameTreeNode> from, int move_code, double priori_prob);
};

struct NeuralGameTreeNode: public std::enable_shared_from_this<NeuralGameTreeNode> {
    Model model;
    std::shared_ptr<ComputationPool> pool;
    int worker_id;

    mech::Configuration config;
    mech::Party root_party;

    double total_advantage = 0;     // W
    int visit_count = 0;            // N
    double priori_prob = 0;         // P

    std::weak_ptr<NeuralGameTreeNode> parent;
    std::deque<SelfPlayData::MoveProb> remaining_moves;
    std::deque<NeuralGameTreeEdge> visited_edges;

    NeuralGameTreeNode(Model model, const mech::Configuration & config, double priori_prob, std::shared_ptr<NeuralGameTreeNode> parent,
    std::shared_ptr<ComputationPool> pool, int worker_id);

    NeuralGameTreeNode(Model model, const mech::Configuration & config, double priori_prob,
    std::shared_ptr<ComputationPool> pool, int worker_id);

    void back_propagate(double advantage);

    std::shared_ptr<NeuralGameTreeNode> select(double c_puct = 1.414);

    void expand();

    void initialize();

    /**
     * @brief Calculate the upper confidence bound of this node.
     * 
     * @return double ucb of this node.
     */
    double ucb(double c_puct = 1.414);

    static std::shared_ptr<NeuralGameTreeNode> create(Model model, const mech::Configuration& root_config,
    std::shared_ptr<ComputationPool> pool, int worker_id);
};

}
}

#endif // !NEURAL_GAME_TREE_HPP
