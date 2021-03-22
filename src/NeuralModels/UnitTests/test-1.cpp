#include <iostream>

#include "NeuralModel.hpp"

int main() {
    auto x = torch::ones({1, 15, 9, 10});
    auto mod = cc4huo::nnmodel::Model();
    std::cout << mod << std::endl;
    auto y = mod.forward(x);
    std::cout << y.first << ' ' << y.second << std::endl;
}