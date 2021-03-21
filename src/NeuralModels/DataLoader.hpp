#ifndef DATA_LOADER_HPP
#define DATA_LOADER_HPP

#include <torch/torch.h>
#include "ChessMechanism.hpp"

namespace cc4huo {
namespace nnmodel {

torch::Tensor convert_configuration_to_input_tensor(const mech::Configuration & config);

}
}

#endif // !DATA_LOADER_HPP
