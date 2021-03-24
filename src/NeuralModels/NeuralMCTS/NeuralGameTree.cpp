#include "NeuralGameTree.hpp"
#include "DataLoader.hpp"
#include <torch/torch.h>

namespace cc4huo {
namespace nnmodel {

NeuralGameTreeEdge::NeuralGameTreeEdge(std::shared_ptr<NeuralGameTreeNode> from, int move_code, double priori_prob)
    : move_code(move_code) {
    static mech::MoveEncoder encoder;

    child = std::make_shared<NeuralGameTreeNode>(
        from->model, 
        from->config.configuration_after_legal_move(encoder.decode(move_code)), 
        priori_prob, 
        from,
        from->pool,
        from->worker_id
    );
    child->initialize();
}

torch::Tensor legimate_distribution(torch::Tensor priori, const std::deque<mech::ChessMove> & legal_moves) {
    static mech::MoveEncoder encoder;

    torch::Tensor mask = torch::zeros({2086});
    for(auto & move : legal_moves) {
        mask[encoder.encode(move)] = 1.0;
    }

    return torch::softmax(priori, 0) * mask;
}

void NeuralGameTreeNode::initialize() {
    auto status = config.status();
    if(status != mech::ACTIVE) {
        if(status == mech::RED_WIN && root_party == mech::RED) {
            back_propagate(1.0);
        } else if(status == mech::BLACK_WIN && root_party == mech::BLACK) {
            back_propagate(-1.0);
        } else {
            back_propagate(0.0);
        }
        return;
    }

    pool->submit(convert_configuration_to_input_tensor(config), worker_id);
    auto evaluated = pool->acquire(worker_id);

    back_propagate(evaluated.second.item().toDouble());

    auto legal_distribution = legimate_distribution(evaluated.first, config.legal_move_list());
    legal_distribution = legal_distribution;
    for(int i = 0; i < 2086; ++i) {
        if(legal_distribution[i].item().toDouble() > 1e-4) {
            remaining_moves.push_back(std::make_pair(i, static_cast<float>(legal_distribution[i].item().toDouble())));
        }
    }
}

void NeuralGameTreeNode::back_propagate(double advantage) {
    total_advantage += advantage;
    visit_count += 1;

    if(parent.lock() == shared_from_this()) {
        return;
    }

    parent.lock()->back_propagate(advantage);
}

NeuralGameTreeNode::NeuralGameTreeNode(
    Model model, const mech::Configuration & config, double priori_prob, 
    std::shared_ptr<NeuralGameTreeNode> parent,
    std::shared_ptr<ComputationPool> pool, int worker_id
)
    : model(model)
    , config(config)
    , root_party(config.active_party)
    , priori_prob(priori_prob)
    , parent(parent)
    , pool(pool)
    , worker_id(worker_id)
{
}

NeuralGameTreeNode::NeuralGameTreeNode(
    Model model, const mech::Configuration & config, double priori_prob,
    std::shared_ptr<ComputationPool> pool, int worker_id
)
    : model(model)
    , config(config)
    , root_party(config.active_party)
    , priori_prob(priori_prob)
    , parent()
    , pool(pool)
    , worker_id(worker_id)
{
}

std::shared_ptr<NeuralGameTreeNode> NeuralGameTreeNode::select(double c_puct) {
    if(!remaining_moves.empty()) {
        return shared_from_this();
    }

    std::shared_ptr<NeuralGameTreeNode> result;
    double max_ucb = -1e9;

    for(auto & e : visited_edges) {
        auto ucb = e.child->ucb(c_puct);
        // std::cout << "ucb = " << ucb << std::endl;
        if(ucb > max_ucb) {
            max_ucb = ucb;
            result = e.child;
        }
    }

    return result == nullptr ? result : result->select(c_puct);
}

void NeuralGameTreeNode::expand() {
    if(remaining_moves.empty()) {
        auto status = config.status();
        if(status == mech::RED_WIN && root_party == mech::RED) {
            back_propagate(1.0);
        } else if(status == mech::BLACK_WIN && root_party == mech::BLACK) {
            back_propagate(-1.0);
        } else {
            back_propagate(0.0);
        }
        return;
    }

    auto move = remaining_moves.front();
    remaining_moves.pop_front();

    // std::cout << "creating new edge from" << shared_from_this().get() << std::endl;
    visited_edges.emplace_back(shared_from_this(), move.first, move.second);
    // std::cout << "created new edge" << std::endl;
}

double NeuralGameTreeNode::ucb(double c_puct) {
    if(parent.lock() == shared_from_this()) {
        throw std::runtime_error("[NeuralGameTreeNode::ucb] UCB has no definition for root node.");
    }

    // std::cout << total_advantage << ' ' << visit_count << ' ' << priori_prob << std::endl;

    double result = total_advantage / visit_count;
    result += c_puct * priori_prob * sqrt(parent.lock()->visit_count) / (1 + visit_count);
    return result;
}

std::shared_ptr<NeuralGameTreeNode> NeuralGameTreeNode::create(Model model, const mech::Configuration& root_config,
std::shared_ptr<ComputationPool> pool, int worker_id) {
    auto result = std::make_shared<NeuralGameTreeNode>(model, root_config, 1.0, pool, worker_id);
    result->parent = result;
    result->initialize();
    // std::cout << "created" << std::endl;
    return result;
}

}
}
