#ifndef AGENT_NEURAL_HPP
#define AGENT_NEURAL_HPP

#include <atomic>
#include <mutex>
#include <memory>
#include <condition_variable>

#include <torch/torch.h>

#include "Agent.hpp"
#include "NeuralModel.hpp"
#include "SelfPlayData.hpp"
#include "NeuralModel.hpp"

namespace cc4huo {
namespace nnmodel {

struct ComputationPool;

struct AgentNeural : public Agent {
    Model model;
    std::shared_ptr<ComputationPool> pool;
    int worker_id;

    AgentNeural(const std::string & model_filename, 
        std::shared_ptr<ComputationPool> pool,
        int worker_id);

    virtual void evaluate(const mech::Configuration & config, std::deque<SelfPlayData::MoveProb> & distribution) override;
};

struct ComputationPool {
    torch::Tensor * inputs;
    torch::Tensor * distribution_outputs;
    torch::Tensor * value_outputs;

    int batch_size;

    std::atomic_int submit_count;
    std::mutex m;
    std::condition_variable condition_output_ready;     // Tree search threads wait on this
    std::condition_variable condition_input_ready;      // Batch computation thread waits on this

    ComputationPool(int batch_size);                    // number of workers must equal batch size!
    ~ComputationPool();

    void submit(torch::Tensor input, int id);

    std::pair<torch::Tensor, torch::Tensor> acquire(int id);

    void wait_until_output_ready();

    void wait_until_input_ready();

    void compute(Model model);

    ComputationPool(const ComputationPool &) = delete;

private:
    ComputationPool operator=(const ComputationPool &);
};

}
}

#endif // !AGENT_NEURAL_HPP
