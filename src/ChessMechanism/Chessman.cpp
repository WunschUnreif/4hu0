#include <exception>
#include <cctype>
#include <algorithm>
#include <iostream>

#include "Chessman.hpp"

namespace cc4huo {
namespace mech {

// ===---------- ChessmanPosition::* ----------=== //

ChessmanPosition ChessmanPosition::operator+ (std::pair<int, int> d) const {
    return ChessmanPosition(x + d.first, y + d.second);
}

bool ChessmanPosition::valid() const {
    return x >= 0 && x <= 8 && y >= 0 && y <= 9;
}

std::string ChessmanPosition::position_name() const {
    return std::to_string(static_cast<char>('a' + x)) + std::to_string(y);
}


// ===---------- Chessman::* ----------=== //

std::deque<std::pair<int, int>> rook_cannon_diffs(ChessmanPosition pos) {
    std::deque<std::pair<int, int>> diffs;

    for(int dx = 0 - pos.x; dx <= 8 - pos.x; dx++) {
        if(dx == 0) continue;
        diffs.push_back(std::make_pair(dx, 0));
    }

    for(int dy = 0 - pos.y; dy <= 9 - pos.y; dy++) {
        if(dy == 0) continue;
        diffs.push_back(std::make_pair(0, dy));
    }

    return diffs;
}

std::deque<std::pair<int, int>> horse_diffs(ChessmanPosition pos) {
    static int moves [8][2] = {
        { 1, 2}, { 1, -2},
        {-1, 2}, {-1, -2},
        { 2, 1}, { 2, -1},
        {-2, 1}, {-2, -1}
    };

    std::deque<std::pair<int, int>> diffs;

    for(int move_index = 0; move_index < 8; ++move_index){
        auto d = std::make_pair(moves[move_index][0], moves[move_index][1]);
        if((pos + d).valid()) {
            diffs.push_back(d);
        }
    }
    
    return diffs;
}

std::deque<std::pair<int, int>> elephant_diffs(ChessmanPosition pos) {
    static int moves [4][2] = {
        { 2, 2}, { 2, -2},
        {-2, 2}, {-2, -2},
    };

    std::deque<std::pair<int, int>> diffs;

    for(int move_index = 0; move_index < 8; ++move_index){
        auto d = std::make_pair(moves[move_index][0], moves[move_index][1]);
        if((pos + d).valid()) {
            diffs.push_back(d);
        }
    }
    
    return diffs;
}

std::deque<std::pair<int, int>> advisor_diffs(ChessmanPosition pos) {
    static int moves [4][2] = {
        { 1, 1}, { 1, -1},
        {-1, 1}, {-1, -1},
    };

    std::deque<std::pair<int, int>> diffs;

    for(int move_index = 0; move_index < 8; ++move_index){
        auto d = std::make_pair(moves[move_index][0], moves[move_index][1]);
        if((pos + d).valid()) {
            diffs.push_back(d);
        }
    }
    
    return diffs;
}

std::deque<std::pair<int, int>> king_diffs(ChessmanPosition pos) {
    static int moves [4][2] = {
        {1, 0}, {-1,  0},
        {0, 1}, { 0, -1},
    };

    std::deque<std::pair<int, int>> diffs;

    for(int move_index = 0; move_index < 8; ++move_index){
        auto d = std::make_pair(moves[move_index][0], moves[move_index][1]);
        if((pos + d).valid()) {
            diffs.push_back(d);
        }
    }
    
    return diffs;
}

std::deque<std::pair<int, int>> pawn_diffs(ChessmanPosition pos, Party party) {
    static auto red_direct      = std::make_pair( 0,  1);
    static auto black_direct    = std::make_pair( 0, -1);
    static auto left            = std::make_pair( 1,  0);
    static auto right           = std::make_pair(-1,  0);

    std::deque<std::pair<int, int>> diffs;

    if(party == RED) {
        if((pos + red_direct).valid()) diffs.push_back(red_direct);
        if(pos.y >= 5) {
            if((pos +  left).valid()) diffs.push_back(left);
            if((pos + right).valid()) diffs.push_back(right);
        }
    } else {
        if((pos + black_direct).valid()) diffs.push_back(black_direct);
        if(pos.y <= 4) {
            if((pos +  left).valid()) diffs.push_back(left);
            if((pos + right).valid()) diffs.push_back(right);
        }
    }
    
    return diffs;
}

std::deque<std::pair<int, int>> Chessman::valid_move_diffs(ChessmanPosition pos) const {
    switch(type) {
    case ROOK       : return rook_cannon_diffs(pos);
    case CANNON     : return rook_cannon_diffs(pos);
    case HORSE      : return horse_diffs(pos);
    case ELEPHANT   : return elephant_diffs(pos);
    case ADVISOR    : return advisor_diffs(pos);
    case KING       : return king_diffs(pos);
    case PAWN       : return pawn_diffs(pos, party);

    default         : throw  std::runtime_error("Invalid chessman");
    }
}


const char CHESSMAN_NAMES[7] = {'k', 'a', 'e', 'h', 'r', 'c', 'p'}; // order must be identical to the declarations in the enum.

char Chessman::chessman_name() const {
    auto name = CHESSMAN_NAMES[type];
    if(party == RED) {
        name = toupper(name);
    }
    return name;
}

Chessman::Chessman(char name) {
    if(isupper(name)) {
        party = RED;
    } else {
        party = BLACK;
    }

    name = tolower(name);

    auto pos = std::find(std::begin(CHESSMAN_NAMES), std::end(CHESSMAN_NAMES), name);

    if(pos == std::end(CHESSMAN_NAMES)) {
        throw std::runtime_error(std::string("Unknown chessman name: ") + name);
    }

    type = static_cast<ChessmanType>(pos - std::begin(CHESSMAN_NAMES));
}

} // mech
} // cc4huo
