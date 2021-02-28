#include "ChessMechanism.hpp"

#include <random>
#include <iostream>
#include <ctime>

using namespace cc4huo::mech;

int main() {
    auto game = Game::new_game();

    std::default_random_engine rand_engine;
    rand_engine.seed(time(NULL));

    while(true) {
        auto move_list = game.current_configuration().legal_move_list();

        if( move_list.size() == 0 || 
            game.current_configuration().board.attackable_chessman_count() == 0 ||
            game.current_configuration().neutral_steps >= 60
        ) break;

        std::uniform_int_distribution<int> distrb(0, move_list.size() - 1);
        auto choice = move_list[distrb(rand_engine)];

        game.commit_legal_move(choice);
    }

    std::cout << game.game_record() << std::endl;

    return 0;
}
