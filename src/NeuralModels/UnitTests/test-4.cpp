#include <iostream>
#include "NNModel.hpp"

using namespace cc4huo;
using namespace nnmodel;

int main () {
    SelfPlayDataSet dataset("../train/train-data/test", 512);
    std::cout << dataset.raw_data.size() << std::endl;
    int win = 0, lose = 0;
    for(auto & x : dataset.raw_data) {
        if(x.value > 0) win++;
        if(x.value < 0) lose++;
    }
    std::cout << "win " << win << " lose " << lose << std::endl;

    Model mod;
    Trainer trainer(mod);

    auto data = dataset.next();

    for(int i = 0; i < 100; ++i) {
        auto loss = 0.0;
        auto cnt = 0;
        while(dataset.has_next()) {
            loss += trainer.train(dataset.next());
            cnt++;
        }
        dataset.data_iter = dataset.raw_data.begin();
        std::cout << "epoch " << i << ": avg loss = " << loss / cnt << std::endl;
    }

    // while(dataset.has_next()) {
        // mod.forward(std::get<0>(dataset.next()));
    // }
}