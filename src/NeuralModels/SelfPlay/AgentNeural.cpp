#include "AgentNeural.hpp"
#include "ChessMechanism.hpp"
#include "NeuralMCTS/NeuralGameTree.hpp"

#include <iostream>

#include <torch/torch.h>

namespace cc4huo {
namespace nnmodel {

AgentNeural::AgentNeural(const std::string & model_filename, 
    std::shared_ptr<ComputationPool> pool,
    int worker_id)
    : pool(pool)
    , worker_id(worker_id) {
    torch::load(model, model_filename);
}

void AgentNeural::evaluate(const mech::Configuration & config, std::deque<SelfPlayData::MoveProb> & distribution) {
    auto game_tree = NeuralGameTreeNode::create(model, config, pool, worker_id);

    for(int _ = 0; _ < 50; ++_) {
        // std::cout << "expanding" << std::endl;
        auto selection = game_tree->select(1.414);
        // std::cout << "selection " << selection.get() << std::endl;
        selection->expand();
        // std::cout << "expanded" << std::endl;
    }

    // std::cout << "finished search" << std::endl;

    float N = game_tree->visit_count;
    for(auto & e : game_tree->visited_edges) {
        // std::cout << e.move_code << " " << e.child->visit_count << std::endl;
        distribution.push_back(std::make_pair(
            e.move_code,
            e.child->visit_count / N
        ));
    }
}

ComputationPool::ComputationPool(int batch_size)
    : batch_size(batch_size)
    , submit_count(0) {
        inputs = new torch::Tensor[batch_size];
        distribution_outputs = new torch::Tensor[batch_size];
        value_outputs = new torch::Tensor[batch_size];
}

ComputationPool::~ComputationPool() {
    delete [] inputs;
    delete [] distribution_outputs;
    delete [] value_outputs;
}

void ComputationPool::wait_until_input_ready() {
    std::unique_lock<std::mutex> lk(m);
    condition_input_ready.wait(lk);
}

void ComputationPool::wait_until_output_ready() {
    std::unique_lock<std::mutex> lk(m);
    condition_output_ready.wait(lk);
}

void ComputationPool::submit(torch::Tensor input, int id) {
    inputs[id] = input;
    auto count = ++submit_count;
    if(count == batch_size) {
        condition_input_ready.notify_all();
    }
    wait_until_output_ready();
}

void ComputationPool::compute(Model model) {
    submit_count.fetch_sub(submit_count.fetch_sub(0));

    std::vector<torch::Tensor> in(batch_size);
    for(int i = 0; i < batch_size; ++i) {
        in[i] = inputs[i];
    }

    // std::clog << "[ComputationPool::compute] Computing..." << std::endl;

    auto result = model->forward(torch::stack(in)
        .to(torch::cuda::is_available() ? torch::kCUDA : torch::kCPU));

    for(int i = 0; i < batch_size; ++i) {
        distribution_outputs[i] = result.first[i].to(torch::kCPU);
        value_outputs[i] = result.second[i].to(torch::kCPU);
    }

    // std::clog << "[ComputationPool::compute] Computation done." << std::endl;

    condition_output_ready.notify_all();
}

std::pair<torch::Tensor, torch::Tensor> ComputationPool::acquire(int id) {
    return std::make_pair(distribution_outputs[id], value_outputs[id]);
}

}
}
