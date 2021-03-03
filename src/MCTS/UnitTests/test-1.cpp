#include "MCTS.hpp"
#include "ChessMechanism.hpp"

#include <iostream>
#include <chrono>

using namespace cc4huo;

mech::ChessMove search(const mech::Configuration & config) {
    static mcts::GameTreeNode::TreePolicy policy = mcts::gen_ucb_policy(1.414);
    auto game_tree = std::make_shared<mcts::GameTreeNode>(config);
    game_tree->parent = game_tree;

    std::chrono::steady_clock::time_point time_start = std::chrono::steady_clock::now();

    int count = 0;

    while(true) {
        std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();
        std::chrono::duration<double> time_used = std::chrono::duration_cast<std::chrono::duration<double>>(time_end - time_start);
        
        if (time_used.count() >= 15) {
            break;
        }

        // std::cout << "select" << std::endl;
        auto selection = game_tree->select(policy);
        // std::cout << "selected" << std::endl;
        selection->expand();
    }

    int max_visit = 0;
    double win = 0;
    mech::ChessMove move(mech::ChessmanPosition(0, 0), mech::ChessmanPosition(0, 0));

    for(auto & e : game_tree->edges) {
        auto visit = e.child->access_count;
        if(visit > max_visit) {
            max_visit = visit;
            move = e.move;
            win = static_cast<double>(e.child->win_count) / e.child->access_count;
        }
    }

    std::cout   << "Search tree height = " << game_tree->height()
                << ", tree size = " << game_tree->node_count()
                << ", visit = " << max_visit
                << ", winning prob = " << win << std::endl;

    return move;
}

int main() {
    // std::cout << "input fen" << std::endl;
    // std::string fen;
    // std::getline(std::cin, fen);

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
