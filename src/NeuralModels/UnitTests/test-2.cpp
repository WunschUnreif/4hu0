#include <iostream>

#include "NeuralModel.hpp"
#include "DataLoader.hpp"
#include "ChessMechanism.hpp"
#include "SelfPlay/SelfPlayData.hpp"

using namespace cc4huo;

int main() {
    auto x = nnmodel::convert_configuration_to_input_tensor(mech::Configuration::init_config());
    std::cout << x << std::endl;

    nnmodel::SelfPlayData data;
    data.distribution.push_back(std::make_pair(100, 0.0345));
    std::cout << data.to_string() << std::endl;
}