#ifndef CHESSBOARD_HPP
#define CHESSBOARD_HPP

#include "Chessman.hpp"
#include "ChessMove.hpp"

#include <string.h>
#include <deque>

namespace cc4huo {
namespace mech {

struct MaybeChessman {
    bool is_chessman;
    Chessman real_chessman;

    MaybeChessman(): is_chessman(false) {}
    MaybeChessman(Chessman m): is_chessman(true), real_chessman(m) {}

    static MaybeChessman None()             { return MaybeChessman(); }
    static MaybeChessman Some(Chessman m)   { return MaybeChessman(m); }
};

struct Chessboard {
    /**
     * @brief The board stored by columns from left to right, each col from bottom to top.
     */
    MaybeChessman board[9][10];
    //                  ^   ^
    //                  x   y

    Chessboard() { for(int x = 0; x < 9; ++x) for(int y = 0; y < 10; ++y) board[x][y] = MaybeChessman::None(); }

    MaybeChessman& operator[] (ChessmanPosition pos);
    MaybeChessman* operator[] (int x);

    /**
     * @brief generate the FEN representation of the board configuration (only the first part)
     * 
     * @return std::string FEN representation, only fitst part
     */
    std::string to_fen() const;

    /**
     * @brief load board configuration from the first part of a FEN string
     * 
     * @param fen first part of the FEN string
     * @return Chessboard the corresponding board configuration
     * @throw std::runtime_error when the FEN string cannot be parsed
     */
    static Chessboard from_fen(const std::string & fen);

    /**
     * @brief print the board in ascii form to the output stream
     * 
     * @param out output stream
     */
    void debug_print(std::ostream & out) const;
    
    /**
     * @brief Generate the move list for the chessman at the given position
     *        without checking the target configuration is been checked.
     * 
     * @param pos position of the chessman
     * @param result valid move for the chessman, or empty if there is no chessman
     */
    void checkless_move_list_for(ChessmanPosition pos, std::deque<ChessMove> & result, ChessmanType as_type = ChessmanType::ERROR) const;

    /**
     * @brief Generate the move list for a given party without checking 
     *        the target configuration is been checked.
     * 
     * @param party party to generate move list
     * @param[out] moves legal moves is attached to it
     */
    void checkless_move_list(Party party, std::deque<ChessMove> & moves) const;

    /**
     * @brief generate the chessboard after a move
     * 
     * @param move the move
     * @return Chessboard chessboard configuration after the move
     */
    Chessboard board_after_move(const ChessMove & move) const;

    /**
     * @brief Commit a legal move to the board
     * 
     * @param move the legal move to commit
     */
    void commit_move(const ChessMove & move);

    /**
     * @brief Check if the given party is being checked by its opponent in the board configuration
     * 
     * @param party the party suspect to be checked
     * @return true being checked
     * @return false not being checked
     */
    bool is_been_checked(Party party) const;

    /**
     * @brief generate the legal move list of the given party
     * 
     * @param party party to generate legal move list
     * @return std::deque<ChessMove> legal move list
     */
    std::deque<ChessMove> legal_move_list(Party party) const;

    /**
     * @brief Count the number of the designated chessman.
     * 
     * @param type chessman type to count
     * @param party party to count
     * @return int the chessman count
     */
    int chessman_count(ChessmanType type, Party party) const;

    /**
     * @brief Count the total number of attackable chessman.
     * 
     * @return int the chessman count
     */
    int attackable_chessman_count() const;

// private:
//!
    void checkless_filter_move_list(std::deque<ChessMove> & list, ChessmanType type) const;


    // These functions requires the move is generated from the valid move list of a chessman,
    // so the basic validity (isolated legal target) is assumed to be true.
    //!

    bool checkless_valid_move_for_rook       (const ChessMove & move) const;
    bool checkless_valid_move_for_horse      (const ChessMove & move) const;
    bool checkless_valid_move_for_elephant   (const ChessMove & move) const;
    bool checkless_valid_move_for_advisor    (const ChessMove & move) const;
    bool checkless_valid_move_for_king       (const ChessMove & move) const;
    bool checkless_valid_move_for_cannon     (const ChessMove & move) const;
    bool checkless_valid_move_for_pawn       (const ChessMove & move) const;


    ChessmanPosition king_position_of(Party party) const;


    bool is_been_checked_by_rook(Party party, ChessmanPosition king_pos) const;
    bool is_been_checked_by_cannon(Party party, ChessmanPosition king_pos) const;
    bool is_been_checked_by_horse(Party party, ChessmanPosition king_pos) const;
    bool is_been_checked_by_pawn(Party party, ChessmanPosition king_pos) const;
};

}
}

#endif // !CHESSBOARD_HPP
