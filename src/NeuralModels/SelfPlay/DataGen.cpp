#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

#include "Agent.hpp"
#include "AgentPureMCTS.hpp"

using namespace cc4huo;

std::chrono::steady_clock::time_point time_start;

void write_data(std::vector<cc4huo::nnmodel::SelfPlayData> & data, const std::string & filename) {
    std::ofstream fileout(filename);
    for(const auto & d : data) {
        fileout << d.to_string() << "\n";
    }
    fileout.close();
}

void gen_data(int num_start, int stride) {
    std::this_thread::sleep_for(std::chrono::milliseconds(num_start));
    nnmodel::AgentPureMCTS agent;

    while(true) {
        std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();
        std::chrono::duration<double> time_used = std::chrono::duration_cast<std::chrono::duration<double>>(time_end - time_start);

        if(time_used.count() >= 60 * 60) {
            break;
        }

        std::cout << "Generating round " + std::to_string(num_start) << std::endl;

        auto result = agent.play();
        write_data(result, "round-" + std::to_string(num_start) + ".data");
        num_start += stride;
    }
}

int main() {
    constexpr int NUM_THREADS = 1;
    std::vector<std::thread> workers;

    time_start = std::chrono::steady_clock::now();

    for(int i = 0; i < NUM_THREADS; ++i) {
        workers.emplace_back(gen_data, i, NUM_THREADS);
    }

    for(auto & t : workers) {
        t.join();
    }
}
