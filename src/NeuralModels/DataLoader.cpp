#include <cstdlib>
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <random>
#include <ctime>

#include "DataLoader.hpp"

namespace cc4huo {
namespace nnmodel {

torch::Tensor extract_chessman_tensor(
    const mech::Configuration & config, 
    mech::ChessmanType type, 
    mech::Party party) {
    auto result = torch::zeros({9, 10});
    for(int x = 0; x < 9; ++x) {
        for(int y = 0; y < 10; ++y) {
            auto piece = config.board.board[x][y];
            if(piece.is_chessman && piece.real_chessman.party == party && piece.real_chessman.type == type) {
                result[x][y] = 1;
            }
        }
    }
    return result;
}

torch::Tensor extract_player_tensor(const mech::Configuration & config) {
    return config.active_party == mech::RED ? torch::ones({9, 10}) : torch::zeros({9, 10});
}

torch::Tensor convert_configuration_to_input_tensor(const mech::Configuration & config) {
    torch::Tensor result = torch::stack({
        extract_chessman_tensor(config, mech::ROOK, mech::BLACK),
        extract_chessman_tensor(config, mech::HORSE, mech::BLACK),
        extract_chessman_tensor(config, mech::ELEPHANT, mech::BLACK),
        extract_chessman_tensor(config, mech::ADVISOR, mech::BLACK),
        extract_chessman_tensor(config, mech::KING, mech::BLACK),
        extract_chessman_tensor(config, mech::CANNON, mech::BLACK),
        extract_chessman_tensor(config, mech::PAWN, mech::BLACK),
        extract_player_tensor(config),
        extract_chessman_tensor(config, mech::ROOK, mech::RED),
        extract_chessman_tensor(config, mech::HORSE, mech::RED),
        extract_chessman_tensor(config, mech::ELEPHANT, mech::RED),
        extract_chessman_tensor(config, mech::ADVISOR, mech::RED),
        extract_chessman_tensor(config, mech::KING, mech::RED),
        extract_chessman_tensor(config, mech::CANNON, mech::RED),
        extract_chessman_tensor(config, mech::PAWN, mech::RED)
    });

    return result;
}


bool maybe_data_file(const std::string & path) {
    return path.length() >= 10 && path.substr(0, 6) == "round-" && path.substr(path.length() - 5, 5) == ".data";
}

std::vector<std::string> scan_path_for_data_file(const std::string & dir) {
    std::vector<std::string> result;
    auto dirp = opendir(dir.c_str());
    dirent * entry;

    while(dirp != NULL && (entry = readdir(dirp)) != NULL) {
        if(maybe_data_file(entry->d_name)) {
            result.push_back(dir + "/" + entry->d_name);
        }
    }
    closedir(dirp);

    return result;
}

SelfPlayDataSet::SelfPlayDataSet(const std::string & dir, int bs): data_dir(dir), batch_size(bs) {
    load_data();
    data_iter = raw_data.begin();
}

bool load_next_data_entry(std::ifstream & data_file, std::vector<SelfPlayData> & buffer) {
    std::string line1, line2, line3;
    if(!std::getline(data_file, line1)) return false;
    if(!std::getline(data_file, line2)) return false;
    if(!std::getline(data_file, line3)) return false;
    buffer.emplace_back(SelfPlayData::from_string(line1, line2, line3));
    return true;
}

void load_data_file(const std::string & filename, std::vector<SelfPlayData> & buffer) {
    std::ifstream data_file(filename);
    while(load_next_data_entry(data_file, buffer));
}

void SelfPlayDataSet::load_data() {
    auto data_filenames = scan_path_for_data_file(data_dir);
    for(auto & name : data_filenames) {
        // std::cout << name << std::endl;
        load_data_file(name, raw_data);
        // std::cout << raw_data.size() << std::endl;
    }
    std::shuffle(raw_data.begin(), raw_data.end(), std::default_random_engine(time(NULL)));
}

bool SelfPlayDataSet::has_next() {
    return raw_data.end() - data_iter >= batch_size;
}

std::tuple<torch::Tensor, torch::Tensor, torch::Tensor> SelfPlayDataSet::next() {
    std::vector<torch::Tensor> layers, distrb, result;

    for(int i = 0; i < batch_size; ++i) {
        layers.emplace_back(get_next_layers());
        distrb.emplace_back(get_next_distrb());
        result.emplace_back(get_next_result());
        data_iter++;
    }

    return std::make_tuple(torch::stack(layers), torch::stack(distrb), torch::stack(result));
}

torch::Tensor SelfPlayDataSet::get_next_layers() {
    return convert_configuration_to_input_tensor(data_iter->config);
}

torch::Tensor SelfPlayDataSet::get_next_distrb() {
    torch::Tensor distrb = torch::zeros({2086});
    for(auto & moveprob : data_iter->distribution) {
        distrb[moveprob.first] = moveprob.second;
    }
    return distrb;
}

torch::Tensor SelfPlayDataSet::get_next_result() {
    torch::Tensor result = torch::zeros({1});
    result[0] = data_iter->value;
    return result;
}

}
}
