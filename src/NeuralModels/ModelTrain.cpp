#include <iostream>
#include <sstream>

#include <cstdlib>
#include <unistd.h>
#include <cctype>

#include <torch/torch.h>

#include "NNModel.hpp"

using namespace cc4huo::nnmodel;

std::string input_params_filename;
std::string output_params_filename;
std::string data_dirname;
int num_epoch;
double learning_rate;


void print_usage() {
    std::cout << 
        "Usage: model-train\n"
        "         -i <original parameters file>\n"
        "         -o <new parameter file>\n"
        "         -d <directory of self play data>\n"
        "         -e <#epoch>\n"
        "         -r <learning rate>" << std::endl;
}

// Usage: model-train 
//          -i <original parameters file> 
//          -o <new parameter file> 
//          -d <directory of self play data> 
//          -e <#epoch> 
//          -r <learning rate>
int main(int argc, char ** argv) {
    bool cuda = torch::cuda::is_available();

    int arg;
    if(argc != 11) {
        print_usage();
        return -1;
    }

    while((arg = getopt(argc, argv, "i:o:d:e:r:")) != -1) {
        switch(arg) {
        case 'i': input_params_filename = std::string(optarg); break;
        case 'o': output_params_filename = std::string(optarg); break;
        case 'd': data_dirname = std::string(optarg); break;
        case 'e': std::istringstream(optarg) >> num_epoch; break;
        case 'r': std::istringstream(optarg) >> learning_rate; break;
        default: print_usage(); return -1;
        }
    }

    Model model;
    torch::load(model, input_params_filename);

    Trainer trainer(model, learning_rate);
    SelfPlayDataSet dataset(data_dirname);

    for(int epoch = 0; epoch < num_epoch; ++epoch) {
        while(dataset.has_next()) {
            trainer.train(dataset.next());
        }
        dataset.data_iter = dataset.raw_data.begin();
    }

    model->to(torch::kCPU);
    torch::save(model, output_params_filename);
    return 0;
}

