#include <iostream>
#include <memory>

#include "NeuralModel.hpp"

namespace cc4huo {
namespace nnmodel {

ResBlock::ResBlock()
    : conv1(register_module(
        "conv1", 
        torch::nn::Conv2d(
            torch::nn::Conv2dOptions(128, 128, 3).stride(1).padding(1)
        )))
    , conv2(register_module(
        "conv2", 
        torch::nn::Conv2d(
            torch::nn::Conv2dOptions(128, 128, 3).stride(1).padding(1)
        )))
    , bn1(register_module("bn1", torch::nn::BatchNorm2d(128)))
    , bn2(register_module("bn2", torch::nn::BatchNorm2d(128)))
    , relu1(register_module("relu1", torch::nn::ReLU(torch::nn::ReLUOptions())))
    , relu2(register_module("relu2", torch::nn::ReLU(torch::nn::ReLUOptions())))
{}

torch::Tensor ResBlock::forward(torch::Tensor x) {
    return relu2(x + bn2(conv2(relu1(bn1(conv1(x))))));
}


PolicyHead::PolicyHead()
    : conv(register_module(
        "conv", 
        torch::nn::Conv2d(
            torch::nn::Conv2dOptions(128, 32, 3).stride(1).padding(1)
        )))
    , bn(register_module("bn", torch::nn::BatchNorm2d(32)))
    , relu(register_module("relu", torch::nn::ReLU(torch::nn::ReLUOptions())))
    , fc(register_module("fc", torch::nn::Linear(32 * 9 * 10, 2086)))
{}

torch::Tensor PolicyHead::forward(torch::Tensor x) {
    return fc(relu(bn(conv(x))).view({-1, 32 * 9 * 10}));
}

ValueHead::ValueHead()
    : conv(register_module(
        "conv", 
        torch::nn::Conv2d(
            torch::nn::Conv2dOptions(128, 32, 3).stride(1).padding(1)
        )))
    , bn(register_module("bn", torch::nn::BatchNorm2d(32)))
    , relu1(register_module("relu1", torch::nn::ReLU(torch::nn::ReLUOptions())))
    , relu2(register_module("relu2", torch::nn::ReLU(torch::nn::ReLUOptions())))
    , fc1(register_module("fc1", torch::nn::Linear(32 * 9 * 10, 128)))
    , fc2(register_module("fc2", torch::nn::Linear(128, 1)))
    , tanh(register_module("tanh", torch::nn::Tanh()))
{}

torch::Tensor ValueHead::forward(torch::Tensor x) {
    return tanh(
        fc2(
            relu2(fc1(
                relu1(bn(conv(x))).view({-1, 32 * 9 * 10}))
            )
        )
    );
    auto ce = torch::nn::CrossEntropyLoss();
}

ModelImpl::ModelImpl()
    : conv(register_module(
        "conv", 
        torch::nn::Conv2d(
            torch::nn::Conv2dOptions(15, 128, 3).stride(1).padding(1)
        )))
    , bn(register_module("bn", torch::nn::BatchNorm2d(128)))
    , relu(register_module("relu", torch::nn::ReLU(torch::nn::ReLUOptions())))
    , policy(register_module("policy", std::make_shared<PolicyHead>()))
    , value(register_module("value", std::make_shared<ValueHead>()))
{
    resmods = register_module("resmods", torch::nn::Sequential(
        ResBlock(), ResBlock(),
        ResBlock(), ResBlock(),
        ResBlock(), ResBlock(),
        ResBlock() /*, ResBlock(),
        ResBlock(), ResBlock(),
        ResBlock(), ResBlock(),
        ResBlock()*/
    ));
}

std::pair<torch::Tensor, torch::Tensor> ModelImpl::forward(torch::Tensor x) {
    torch::Tensor after_res = resmods->forward(relu(bn(conv(x))));
    return std::make_pair(policy->forward(after_res), value->forward(after_res));
}

}
}
