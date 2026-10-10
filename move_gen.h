#pragma once
#include "board.h"
#include "stack_vector.h"

//type alias for movelist
using MoveList = StackVector<Move, 256>;

namespace MoveGen {
	void generate_pseudo_legal_moves(const Board& board, MoveList& list);
	bool is_under_attack(const Board& board, int square);
	void generate_legal_moves(Board& board, MoveList& legal_move_list);

}