#include <sstream>

#include "SelfPlayData.hpp"

namespace cc4huo {
namespace nnmodel {

std::string SelfPlayData::to_string() const {
    std::string result = config.to_fen() + "\n";
    for(const auto & moveprob : distribution) {
        result += std::to_string(moveprob.first) + " " + std::to_string(moveprob.second) + " ";
    }
    result += (value == 0 ? "\n0" : (value == 1 ? "\n+" : "\n-"));
    return result;
}

SelfPlayData SelfPlayData::from_string(const std::string & line1, const std::string & line2, const std::string & line3) {
    SelfPlayData result;
    result.config = mech::Configuration::from_fen(line1);

    if(line3[0] == '0') result.value = 0;
    else if(line3[0] == '+') result.value = 1;
    else result.value = -1;

    std::istringstream ss(line2);
    int move_code;
    float prob;
    while(ss >> move_code >> prob) {
        result.distribution.push_back(std::make_pair(move_code, prob));
    }

    return result;
}

}
}
