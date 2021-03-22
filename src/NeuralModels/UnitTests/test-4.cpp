#include <iostream>
#include "NNModel.hpp"

using namespace cc4huo;
using namespace nnmodel;

int main () {
    SelfPlayDataSet dataset("../train/train-data/test", 1);
    std::cout << dataset.raw_data.size() << std::endl;
    int win = 0, lose = 0;
    for(auto & x : dataset.raw_data) {
        if(x.value > 0) win++;
        if(x.value < 0) lose++;
    }
    std::cout << "win " << win << " lose " << lose << std::endl;

    Model mod;
    // while(dataset.has_next()) {
        mod.forward(std::get<0>(dataset.next()));
    // }
}