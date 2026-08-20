#pragma once
#include "Move.h"
#include "utils.h"
#include "stack_vector.h"
/*
 10*12 board
0,   1,   2,   3,   4,   5,   6,   7,   8,   9,
10,  11,  12,  13,  14,  15,  16,  17,  18,  19,
20,  21,  22,  23,  24,  25,  26,  27,  28,  29,
30,  31,  32,  33,  34,  35,  36,  37,  38,  39,
40,  41,  42,  43,  44,  45,  46,  47,  48,  49,
50,  51,  52,  53,  54,  55,  56,  57,  58,  59,
60,  61,  62,  63,  64,  65,  66,  67,  68,  69,
70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
80,  81,  82,  83,  84,  85,  86,  87,  88,  89,
90,  91,  92,  93,  94,  95,  96,  97,  98,  99,
100, 101, 102, 103, 104, 105, 106, 107 ,108 ,109,
110, 111, 112, 113, 114, 115, 116, 117, 118, 119

---------10*12 playbing board--------
[8] 21,  22,  23,  24,  25,  26,  27,  28,
[7] 31,  32,  33,  34,  35,  36,  37,  38,
[6] 41,  42,  43,  44,  45,  46,  47,  48,
[5] 51,  52,  53,  54,  55,  56,  57,  58,
[4] 61,  62,  63,  64,  65,  66,  67,  68,
[3] 71,  72,  73,  74,  75,  76,  77,  78,
[2] 81,  82,  83,  84,  85,  86,  87,  88,
[1] 91,  92,  93,  94,  95,  96,  97,  98,
    [A]  [B]  [C]  [D]  [E]  [F]  [G]  [H]
*/

struct UndoState {
	uint8_t castling_rights;
	int halfmove_clock;
	int en_passant_square;

	UndoState() = default;

	UndoState(uint8_t cr, int ep, int hm)
		:castling_rights(cr), en_passant_square(ep), halfmove_clock(hm) {}
};

//
using UndoHistory = StackVector<UndoState, 256>;

class chess_board {
private:
	//borad
	pieces position_board[120];
	colours side_to_move;
	UndoHistory history_stack;
	int black_king_square;
	int white_king_square;
	uint8_t castling_rights;
	int en_passant_square;
	int halfmove_clock;

public:
	//inline function 
	colours get_side_to_move() const { return side_to_move; }
	pieces get_piece(int index) const { return position_board[index]; }
	int get_king_square(colours colour) const { return colour == colours::WHITE ? white_king_square : black_king_square ; }
	int get_en_passant_square() const { return this->en_passant_square; }
	uint8_t get_castling_rights() const { return castling_rights; }
	int get_halfmove_clock() const { return this->halfmove_clock; }

	void clear_board();
	int get_castling_mask(int var) {
		switch (var) {
		case 95:
			return 12;
		case 25:
			return 3;
		case 98:
			return 14;
		case 28:
			return 11;
		case 91:
			return 13;
		case 21:
			return 7;
		default:
			return 15;
		}
	}
	chess_board();//constructor
	void print_board()const;
	void reset_board();
	void Make_move(const Move& move);
	void unmake_move(const Move& move);

	//
	void load_fen(const std::string& fen);
};
