#include "MCTS.hpp"
#include "ChessMechanism.hpp"

#include <iostream>
#include <chrono>

using namespace cc4huo;

mech::ChessMove search(const mech::Configuration & config) {
    mcts::GameTreeNode::TreePolicy policy = mcts::gen_ucb_policy(1.414);
    auto game_tree = std::make_shared<mcts::GameTreeNode>(config);
    game_tree->parent = game_tree;

    std::chrono::steady_clock::time_point time_start = std::chrono::steady_clock::now();

    int count = 0;

    while(true) {
        std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();
        std::chrono::duration<double> time_used = std::chrono::duration_cast<std::chrono::duration<double>>(time_end - time_start);
        
        if (time_used.count() >= 120) {
            break;
        }

        auto selection = game_tree->select(policy);
        selection->expand();
    }

    double max_win = 0;
    mech::ChessMove move(mech::ChessmanPosition(0, 0), mech::ChessmanPosition(0, 0));

    for(auto & e : game_tree->edges) {
        auto win = static_cast<double>(e.child->win_count) / (e.child->access_count == 0 ? 1 : e.child->access_count);
        if(win > max_win) {
            max_win = win;
            move = e.move;
        }
    }

    std::cout << "Search tree height = " << game_tree->height() << ", winning prob = " << max_win << std::endl;

    return move;
}

int main() {
    mech::Game game = mech::Game::new_game();

    while(game.is_ended() == false) {
        auto move = search(game.current_configuration());
        game.commit_legal_move(move);

        std::cout << move.move_name() << std::endl;
        game.current_configuration().board.debug_print(std::cout);
        std::cout << "-----------------" << std::endl;
    }

    std::cout << game.game_record() << std::endl;

    return 0;
}
