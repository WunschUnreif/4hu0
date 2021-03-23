#include <iostream>
#include <chrono>

#include "NeuralModel.hpp"

int main() {
    auto x = torch::rand({1, 15, 9, 10});
    auto mod = cc4huo::nnmodel::Model();
    std::cout << mod << std::endl;

    auto start = std::chrono::steady_clock::now();
    for(int i = 0; i < 100; ++i) {
        auto y = mod.forward(x);
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "elapsed: " << elapsed.count() << std::endl;
}