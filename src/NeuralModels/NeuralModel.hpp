#ifndef NEURAL_MODEL_HPP
#define NEURAL_MODEL_HPP

#include <torch/torch.h>
#include <utility>

namespace cc4huo {
namespace nnmodel {

struct ResBlock: public torch::nn::Module {
    ResBlock();
    torch::Tensor forward(torch::Tensor x);

private:
    torch::nn::Conv2d conv1, conv2;
    torch::nn::BatchNorm2d bn1, bn2;
    torch::nn::ReLU relu1, relu2;
};

struct PolicyHead: public torch::nn::Module {
    PolicyHead();
    torch::Tensor forward(torch::Tensor x);

private:
    torch::nn::Conv2d conv;
    torch::nn::BatchNorm2d bn;
    torch::nn::ReLU relu;
    torch::nn::Linear fc;
};

struct ValueHead: public torch::nn::Module {
    ValueHead();
    torch::Tensor forward(torch::Tensor x);

private:
    torch::nn::Conv2d conv;
    torch::nn::BatchNorm2d bn;
    torch::nn::ReLU relu1, relu2;
    torch::nn::Linear fc1, fc2;
    torch::nn::Tanh tanh;
};

struct Model: public torch::nn::Module {
    Model();
    std::pair<torch::Tensor, torch::Tensor> forward(torch::Tensor x);

private:
    torch::nn::Conv2d conv;
    torch::nn::BatchNorm2d bn;
    torch::nn::ReLU relu;
    torch::nn::Sequential resmods;
    std::shared_ptr<PolicyHead> policy;
    std::shared_ptr<ValueHead> value;
};

}
}

#endif // !NEURAL_MODEL_HPP