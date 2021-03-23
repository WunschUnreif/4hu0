#include "Trainer.hpp"

namespace cc4huo {
namespace nnmodel {

Trainer::Trainer(Model & model, double lr, double momentum, double weight_decay)
    : learning_rate(lr)
    , momentum(momentum)
    , weight_decay(weight_decay)
    , model(model)
    , optimizer(
        model.parameters(), 
        torch::optim::SGDOptions(lr)
            .momentum(momentum)
            .weight_decay(weight_decay)
    )
    , cuda(torch::cuda::is_available())
{
    if(cuda) {
        model.to(torch::kCUDA);
    }
}

torch::Tensor Trainer::calc_l2_loss() {
    torch::Tensor loss = torch::tensor(0.0).to(cuda ? torch::kCUDA : torch::kCPU);
    auto params = model.parameters();
    for(auto it = params.begin(); it != params.end(); ++it) {
        loss += torch::norm(*it, 2);
    }
    return loss * weight_decay;
}

torch::Tensor cross_entropy(torch::Tensor p, torch::Tensor q) { // p = pred, q = target
    return torch::mean(torch::sum(- q * torch::log(torch::softmax(p, 1)), 1));
}

double Trainer::train(std::tuple<torch::Tensor, torch::Tensor, torch::Tensor> batch_data) {
    static torch::nn::CrossEntropyLoss ce_loss;
    static torch::nn::MSELoss mse_loss;
    mse_loss->to(cuda ? torch::kCUDA : torch::kCPU);
    optimizer.zero_grad();

    if(cuda) {
        std::get<0>(batch_data) = std::get<0>(batch_data).cuda();
        std::get<1>(batch_data) = std::get<1>(batch_data).cuda();
        std::get<2>(batch_data) = std::get<2>(batch_data).cuda();
    }

    auto net_output = model.forward(std::get<0>(batch_data));
    // std::cout << net_output.first.size(0) << std::endl;
    // std::cout << std::get<1>(batch_data) << std::endl;
    
    auto loss = cross_entropy(net_output.first, std::get<1>(batch_data)) + 
                mse_loss->forward(net_output.second, std::get<2>(batch_data));

    loss.backward();
    optimizer.step();

    return (loss + calc_l2_loss()).item().toDouble();
}

}
}