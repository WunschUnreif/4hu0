#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <atomic>

#include "Agent.hpp"
#include "AgentPureMCTS.hpp"

using namespace cc4huo;

std::string output_dir;
std::atomic_int32_t rounds_cnt(0);
int desired_rounds = 120;
int num_workers = 1;

void write_data(std::vector<cc4huo::nnmodel::SelfPlayData> & data, const std::string & filename) {
    std::ofstream fileout(filename);
    for(const auto & d : data) {
        fileout << d.to_string() << "\n";
    }
    fileout.close();
}

void gen_data(int num_start) {
    std::this_thread::sleep_for(std::chrono::milliseconds(num_start));
    nnmodel::AgentPureMCTS agent;

    while(true) {
        int current_rounds = ++rounds_cnt;
        if(current_rounds > desired_rounds) {
            break;
        }

        std::cout << "Generating round " + std::to_string(current_rounds) << std::endl;

        auto result = agent.play();
        write_data(result, output_dir + "/round-" + std::to_string(current_rounds) + ".data");

        std::cout << "Generated round " + std::to_string(current_rounds) << std::endl;
    }

    std::cout << "Generator " + std::to_string(num_start) + " done." << std::endl;
}

int main(int argc, const char ** argv) {
    if(argc != 4) {
        std::cout << "Usage: selfplay-datagen <#threads> <#rounds> <output-directory>" << std::endl;
        return -1;
    }

    std::istringstream num_thread_reader(argv[1]);
    num_thread_reader >> num_workers;

    std::istringstream num_rounds_reader(argv[2]);
    num_rounds_reader >> desired_rounds;

    output_dir = argv[3];

    std::vector<std::thread> workers;

    for(int i = 0; i < num_workers; ++i) {
        workers.emplace_back(gen_data, i);
    }

    for(auto & t : workers) {
        t.join();
    }
}
