#include <random>
#include <ctime>
#include <iostream>
#include <cmath>

#include "Agent.hpp"

namespace cc4huo {
namespace nnmodel {

void softmax_to_distribution(std::vector<SelfPlayData::MoveProb> & distribution) {
    double normalize_denominator = 0;
    const double temperature = 0.5;
    for(auto & moveprob : distribution) {
        normalize_denominator += expf(20.0 * pow(moveprob.second, temperature));
    }
    for(auto & moveprob : distribution) {
        moveprob.second = expf(20.0 * pow(moveprob.second, temperature)) / normalize_denominator;
        std::cout << moveprob.second << std::endl;
    }
    std::cout << normalize_denominator << std::endl;
}

mech::ChessMove max_from_distribution(std::vector<SelfPlayData::MoveProb> & distribution) {
    static mech::MoveEncoder encoder;
    
    float max = 0;
    int sample = distribution[0].first;
    for(auto & moveprob : distribution) {
        if(moveprob.second > max) {
            max = moveprob.second;
            sample = moveprob.first;
        }
    }

    return encoder.decode(sample);
}

mech::ChessMove sample_from_distribution(std::vector<SelfPlayData::MoveProb> distribution) {
    static std::default_random_engine e(time(NULL));
    static mech::MoveEncoder encoder;

    float accumulated = 0;
    for(int i = 0; i < distribution.size(); ++i) {
        distribution[i].second *= 100;
        distribution[i].second += accumulated;
        accumulated = distribution[i].second;
    }

    std::uniform_real_distribution<float> u(0, accumulated);

    auto sample_f = u(e);

    int sample = distribution[0].first;
    for(int i = 0; i < distribution.size(); ++i) {
        if(sample_f < distribution[i].second) {
            sample = distribution[i].first;
            break;
        }
    }

    return encoder.decode(sample);
}

std::vector<SelfPlayData> Agent::play() {
    std::vector<SelfPlayData> result;

    mech::Game game = mech::Game::new_game();
    while(!game.is_ended()) {
        game.current_configuration().board.debug_print(std::cout);
        result.emplace_back();
        result.back().config = game.current_configuration();
        this->evaluate(game.current_configuration(), result.back().distribution);
        softmax_to_distribution(result.back().distribution);
        auto move = sample_from_distribution(result.back().distribution);
        game.commit_legal_move(move);
    }

    auto status = game.current_configuration().status();
    auto value = 0;
    if(status == mech::RED_WIN) {
        value = 1;
    }
    if(status == mech::BLACK_WIN) {
        value = -1;
    }

    for(auto & data : result) {
        data.value = value;
    }

    return result;
}

}
}
