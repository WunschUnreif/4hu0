#include "NNModel.hpp"
#include <thread>

using namespace cc4huo;

int computation_thread(std::shared_ptr<nnmodel::ComputationPool> pool, nnmodel::Model model) {
    while(true) {
        pool->wait_until_input_ready();
        pool->compute(model);
    }
}

int main() {
    auto pool = std::make_shared<nnmodel::ComputationPool>(1);
    nnmodel::AgentNeural neural_agent("../train/models/initial-model.pt", pool, 0);
    auto model = neural_agent.model;

    std::thread t(computation_thread, pool, model);

    auto datas = neural_agent.play();
    for(auto & data : datas) {
        std::cout << data.to_string() << std::endl;
    }
}
