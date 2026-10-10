#pragma once
#include "move_gen.h"
#include "evaluation.h"

namespace search {
	struct SearchResult {
		int score;
		Move best_move;

		SearchResult() :score(-100000), best_move() {};
		SearchResult(int s, Move m) :score(s), best_move(m) {};
	};

	MoveList move_ordering(const Board &board, MoveList &list, Move prev_best_move);
	SearchResult search(Board& board, int depth, int alpha, int beta, int& node_count);
	SearchResult negamax_search(Board& board, int depth, int alpha, int beta, int& node_count, MoveList list);
	SearchResult quiescence(Board& board, int alpha, int beta);
}