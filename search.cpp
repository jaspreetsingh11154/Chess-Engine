#include "search.h"

namespace search {
	//negamax search
	SearchResult negamax_search(chess_board& board, int depth, int alpha, int beta, int& node_count) {
		SearchResult result;
		MoveList list;
		MoveGen::generate_legal_moves(board, list);

		if (depth == 0) {
			node_count += 1;
			return SearchResult(Eval::get_evaluation(board), Move());
		}
		for (const Move& move : list) {
			board.Make_move(move);
			SearchResult current = negamax_search(board, depth - 1, alpha, beta, node_count);
			int score = -current.score;

			board.unmake_move(move);
			if (result.score < score) {
				result.score = score;
				result.best_move = move;
			}
		}
		return result;
	}

	//
}