#include <exception>
#include <cctype>
#include <algorithm>
#include <iostream>

#include "Chessman.hpp"

namespace cc4huo {
namespace mech {

// ===---------- Party ----------=== //

Party opponent_party_of(Party party) {
    if(party == BLACK) return RED;
    else return BLACK;
}


// ===---------- ChessmanType ----------=== //

bool is_attackable_chessman_type(ChessmanType type) {
    return type == ROOK || type == HORSE || type == CANNON || type == PAWN;
}


// ===---------- ChessmanPosition::* ----------=== //

ChessmanPosition ChessmanPosition::operator+ (std::pair<int, int> d) const {
    return ChessmanPosition(x + d.first, y + d.second);
}

bool ChessmanPosition::valid() const {
    return x >= 0 && x <= 8 && y >= 0 && y <= 9;
}

std::string ChessmanPosition::position_name() const {
    return std::string() + static_cast<char>('a' + x) + std::to_string(y);
}


// ===---------- Chessman::* ----------=== //

void rook_cannon_diffs(ChessmanPosition pos, std::deque<std::pair<int, int>> & diffs) {
    for(int dx = 0 - pos.x; dx <= 8 - pos.x; dx++) {
        if(dx == 0) continue;
        diffs.push_back(std::make_pair(dx, 0));
    }

    for(int dy = 0 - pos.y; dy <= 9 - pos.y; dy++) {
        if(dy == 0) continue;
        diffs.push_back(std::make_pair(0, dy));
    }
}

void horse_diffs(ChessmanPosition pos, std::deque<std::pair<int, int>> & diffs) {
    static int moves [8][2] = {
        { 1, 2}, { 1, -2},
        {-1, 2}, {-1, -2},
        { 2, 1}, { 2, -1},
        {-2, 1}, {-2, -1}
    };

    for(int move_index = 0; move_index < 8; ++move_index){
        auto d = std::make_pair(moves[move_index][0], moves[move_index][1]);
        if((pos + d).valid()) {
            diffs.push_back(d);
        }
    }
}

bool valid_elephant_target(ChessmanPosition target, Party party) {
    if(party == RED) {
        if(target.x % 4 == 0) {
            return target.y == 2;
        }
        if(target.x % 2 == 0) {
            return target.y == 0 || target.y == 4;
        }
        return false;
    } else {
        if(target.x % 4 == 0) {
            return target.y == 7;
        }
        if(target.x % 2 == 0) {
            return target.y == 5 || target.y == 9;
        }
        return false;
    }
}

void elephant_diffs(ChessmanPosition pos, Party party, std::deque<std::pair<int, int>> & diffs) {
    static int moves [4][2] = {
        { 2, 2}, { 2, -2},
        {-2, 2}, {-2, -2},
    };

    for(int move_index = 0; move_index < 4; ++move_index){
        auto d = std::make_pair(moves[move_index][0], moves[move_index][1]);
        if((pos + d).valid() && valid_elephant_target(pos + d, party)) {
            diffs.push_back(d);
        }
    }
}

bool valid_advisor_target(ChessmanPosition target, Party party) {
    if(party == RED) {
        if(target.x == 4) {
            return target.y == 1;
        }
        if(target.x == 3 || target.x == 5) {
            return target.y == 0 || target.y == 2;
        }
        return false;
    } else {
        if(target.x == 4) {
            return target.y == 8;
        }
        if(target.x == 3 || target.x == 5) {
            return target.y == 7 || target.y == 9;
        }
        return false;
    }
}

void advisor_diffs(ChessmanPosition pos, Party party, std::deque<std::pair<int, int>> & diffs) {
    static int moves [4][2] = {
        { 1, 1}, { 1, -1},
        {-1, 1}, {-1, -1},
    };

    for(int move_index = 0; move_index < 4; ++move_index){
        auto d = std::make_pair(moves[move_index][0], moves[move_index][1]);
        if((pos + d).valid() && valid_advisor_target(pos + d, party)) {
            diffs.push_back(d);
        }
    }    
}

bool valid_king_target(ChessmanPosition target, Party party) {
    if(party == RED) {
        return target.x >= 3 && target.x <= 5 && target.y >= 0 && target.y <= 2;
    } else {
        return target.x >= 3 && target.x <= 5 && target.y >= 7 && target.y <= 9;
    }
}

void king_diffs(ChessmanPosition pos, Party party, std::deque<std::pair<int, int>> & diffs) {
    static int moves [4][2] = {
        {1, 0}, {-1,  0},
        {0, 1}, { 0, -1},
    };

    for(int move_index = 0; move_index < 4; ++move_index){
        auto d = std::make_pair(moves[move_index][0], moves[move_index][1]);
        if((pos + d).valid() && valid_king_target(pos + d, party)) {
            diffs.push_back(d);
        }
    }    
}

void pawn_diffs(ChessmanPosition pos, Party party, std::deque<std::pair<int, int>> & diffs) {
    static auto red_direct      = std::make_pair( 0,  1);
    static auto black_direct    = std::make_pair( 0, -1);
    static auto left            = std::make_pair( 1,  0);
    static auto right           = std::make_pair(-1,  0);

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
}

void Chessman::valid_move_diffs(ChessmanPosition pos, std::deque<std::pair<int, int>> & diffs) const {
    switch(type) {
    case ROOK       : return rook_cannon_diffs(pos, diffs);
    case CANNON     : return rook_cannon_diffs(pos, diffs);
    case HORSE      : return horse_diffs(pos, diffs);
    case ELEPHANT   : return elephant_diffs(pos, party, diffs);
    case ADVISOR    : return advisor_diffs(pos, party, diffs);
    case KING       : return king_diffs(pos, party, diffs);
    case PAWN       : return pawn_diffs(pos, party, diffs);

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
