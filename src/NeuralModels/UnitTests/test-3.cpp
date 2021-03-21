#include "NNModel.hpp"

using namespace cc4huo;

int main() {
    nnmodel::Agent * agent;
    nnmodel::AgentPureMCTS mcts_agent;
    agent = &mcts_agent;
    auto datas = agent->play();
    for(auto & data : datas) {
        std::cout << data.to_string() << std::endl;
    }
}
