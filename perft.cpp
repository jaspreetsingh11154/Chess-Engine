#include "perft.h"
#include "MoveGen.h"
#include <iostream>

uint64_t perft(chess_board& board, int depth) {
	if (depth <= 0) return 1;
	uint64_t leaf_node_counter = 0;
	MoveList move_list;
	MoveGen::generate_legal_moves(board, move_list);
	for (const Move& move : move_list) {
		board.Make_move(move);
		leaf_node_counter += perft(board, depth - 1);
		board.unmake_move(move);
	}
	return leaf_node_counter;

}

uint64_t perft_divide(chess_board& board, int depth) {
	if (depth <= 0) return 1;
	uint64_t node_counter = 0;
	MoveList move_list;
	MoveGen::generate_legal_moves(board, move_list);
	for (const Move& move : move_list) {
		board.Make_move(move);
		if (move.promoted_piece != pieces::EMPTY) {
			std::cout << move_to_notation(move) << piece_to_notation(move.promoted_piece);
		}
		else std::cout << move_to_notation(move);

		uint64_t sub_nodes = perft(board, depth - 1);
		std::cout << ": " << sub_nodes << "\n";
		node_counter += sub_nodes;
		board.unmake_move(move);
	}
	return node_counter;

}
