#include "torch/torch.h"
#include <iostream>

int main() {
  torch::Tensor tensor = torch::eye(4);
  std::cout << tensor << std::endl;

  return 0;
}
