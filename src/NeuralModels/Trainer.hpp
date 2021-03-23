#ifndef TRAINER_HPP
#define TRAINER_HPP

#include <torch/torch.h>
#include "NeuralModel.hpp"

namespace cc4huo {
namespace nnmodel {

struct Trainer {
    double learning_rate;
    double momentum;
    double weight_decay;
    bool cuda;
 
    torch::optim::SGD optimizer;

    Model model;

    Trainer(Model model, double lr = 0.001, double momentum = 0.9, double weight_decay = 1e-5);

    /**
     * @brief Train the model with a batch
     * 
     * @param batch_data a tuple of <input layers, expected distribution, game result>
     * @return double training loss
     */
    double train(std::tuple<torch::Tensor, torch::Tensor, torch::Tensor> batch_data);

    torch::Tensor calc_l2_loss();
};

}
}

#endif // !TRAINER_HPP
