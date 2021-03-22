#ifndef DATA_LOADER_HPP
#define DATA_LOADER_HPP

#include <vector>
#include <string>
#include <tuple>
#include <algorithm>

#include <torch/torch.h>

#include "ChessMechanism.hpp"
#include "SelfPlay/SelfPlayData.hpp"

namespace cc4huo {
namespace nnmodel {

torch::Tensor convert_configuration_to_input_tensor(const mech::Configuration & config);

struct SelfPlayDataSet {
    int batch_size = 512;
    std::string data_dir;
    std::vector<SelfPlayData> raw_data;
    std::vector<SelfPlayData>::iterator data_iter;

    SelfPlayDataSet(const std::string & dir, int bs = 512);

    bool has_next();

    /**
     * @brief Get next batch.
     * 
     * @return std::tuple<torch::Tensor, Torch::Tensor, Torch::Tensor> (input layers, moveprob distributions, winning results)
     */
    std::tuple<torch::Tensor, torch::Tensor, torch::Tensor> next();

// private:
    void load_data();

    torch::Tensor get_next_layers();
    torch::Tensor get_next_distrb();
    torch::Tensor get_next_result();
};

}
}

#endif // !DATA_LOADER_HPP
