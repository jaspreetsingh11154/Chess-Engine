#pragma once
#include "MoveGen.h"
#include "Evaluation.h"

namespace search {
	struct SearchResult {
		int score;
		Move best_move;

		SearchResult() :score(-100000), best_move() {};
		SearchResult(int s, Move m) :score(s), best_move(m) {};
	};

	SearchResult negamax_search(chess_board& board, int depth, int alpha, int beta, int& node_count);
}