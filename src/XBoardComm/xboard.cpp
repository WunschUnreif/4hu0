#include <iostream>
#include <fstream>
#include <cctype>

#include "ChessMechanism.hpp"
#include "MCTS.hpp"

using namespace cc4huo;

mech::Game game;

void record(const std::string & command, bool _ = false);

mech::ChessMove search(const mech::Configuration & config) {
    record("thinking");
    static mcts::GameTreeNode::TreePolicy policy = mcts::gen_ucb_policy(1.414);
    auto game_tree = std::make_shared<mcts::GameTreeNode>(config);
    game_tree->parent = game_tree;

    std::chrono::steady_clock::time_point time_start = std::chrono::steady_clock::now();

    int count = 0;

    while(true) {
        std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();
        std::chrono::duration<double> time_used = std::chrono::duration_cast<std::chrono::duration<double>>(time_end - time_start);
        
        if (time_used.count() >= 90) {
            break;
        }

        // std::cout << "select" << std::endl;
        auto selection = game_tree->select(policy);

        if(selection->is_always_win) {
            record("root is always win");
            break;
        }
        
        // std::cout << "selected" << std::endl;
        selection->expand();
        // std::cout << "expanded" << std::endl;
    }

    int max_visit = 0;
    double win = 0;
    mech::ChessMove move(mech::ChessmanPosition(0, 0), mech::ChessmanPosition(0, 0));

    for(auto & e : game_tree->edges) {
        auto visit = e.child->access_count;
        if(e.child->is_always_win) {
            record("Search tree height = " + std::to_string(game_tree->height())
                + ", tree size = " +std::to_string(game_tree->node_count())
                + ", always win!"
            );
            return e.move;
        }

        if(visit > max_visit) {
            max_visit = visit;
            move = e.move;
            win = static_cast<double>(e.child->win_count) / e.child->access_count;
        }
    }

    record(    "Search tree height = " + std::to_string(game_tree->height())
                + ", tree size = " + std::to_string(game_tree->node_count())
                + ", visit = " + std::to_string(max_visit)
                + ", winning prob = " + std::to_string(win)
    );

    return move;
}

void record(const std::string & command, bool from_engine) {
    static std::ofstream command_output_file("xboard.txt");
    if(from_engine) {
        command_output_file << "> " << command << std::endl;
    } else {
        command_output_file << command << std::endl;
    }
}

bool is_move_cmd(const std::string & move) {
    if( move.length() >= 4 &&
        isalpha(move[0]) &&
        isnumber(move[1]) &&
        isalpha(move[2]) &&
        isnumber(move[3])
    ) {
            return true;
    }
    return false;
}

void deal_move(const std::string & movestr) {
    game.commit_legal_move(mech::ChessMove(movestr));
    void move_piece();
    move_piece();
}

void move_piece() {
    auto move = search(game.current_configuration());
    record("Move " + move.move_name() + " commmitted");
    std::cout << "move " << move.move_name() << std::endl;
    game.commit_legal_move(move);
}

void deal_command(std::string & command) {
    auto first_word = command.substr(0, command.find_first_of(' '));

    if(first_word == "protover") {
        std::cout   << "feature ping=1 setboard=1 playother=1 reuse=1 myname=\"4hu0\" variants=\"xiangqi\" colors=0 sigint=0" 
                    << std::endl;
    } else if(first_word == "new") {
        game = mech::Game::new_game();
    } else if(is_move_cmd(command)) {
        deal_move(command);
    } else if(first_word == "go") {
        move_piece();
    } else if(first_word == "ping") {
        command[1] = 'o';
        std::cout << command << std::endl;
    }

}

int main() {
    std::string command;
    while(true) {
        std::getline(std::cin, command);
        
        record(command, true);
        deal_command(command);
    }

    return 0;
}
