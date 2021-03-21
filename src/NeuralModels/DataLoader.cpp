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

}
}
