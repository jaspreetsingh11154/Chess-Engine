#include "search.h"
#include <iostream>

namespace search {

	//move odering MVV-LVA(Most valuable victim - least valuable attacker)
	MoveList move_ordering(const Board &board, MoveList &list, Move prev_best_move) {
		for (int i = 0; i < list.size(); i++) {
			int max_score = -100000;
			int move_idx = 0;
			for (int j = i; j < list.size(); j++) {
				int curr_score = 0;
				if (list[j].captured_piece != Pieces::EMPTY) {
					//Eval attacker value
					int attacker_index = (int)board.get_piece(list[j].move_from) - 1;
					attacker_index = attacker_index >= 6 ? attacker_index - 6 : attacker_index;
					int attacker_value = Eval::piece_value[attacker_index];
					
					//Eval victim value
					int captured_piece_index = (int)list[j].captured_piece - 1;
					captured_piece_index -= captured_piece_index >= 6 ? 6 : 0;
					int victim_value = Eval::piece_value[captured_piece_index];

					
					curr_score = victim_value - attacker_value;
				}


				//setting priority for the last best move
				if (prev_best_move == list[j]) {
					std::cout << "\nPritority prev_best_move " << move_to_notation(list[j]);
					curr_score = 1000000;
				}
				if (curr_score > max_score) {
					max_score = curr_score;
					move_idx = j;
				}
			}
			//swap
			auto temp = list[i];
			list[i] = list[move_idx];
			list[move_idx] = temp;

		}
		return list;
	}

	//iterative depth
	SearchResult search(Board& board, int max_depth, int alpha, int beta, int& node_count) {
		SearchResult result;

		for (int depth = 1; depth <= max_depth; depth++) {
			MoveList list;
			MoveGen::generate_legal_moves(board, list);
			move_ordering(board, list, result.best_move);
			std::cout << "\n------------------------------------------------------------------------\n";
			std::cout << "|\t\tAt Depth[" << depth << "] prev_best_move " << move_to_notation(result.best_move);
			std::cout << "\n------------------------------------------------------------------------\n";
			result = negamax_search(board, depth, alpha, beta, node_count, list);

		}
		return result;

	}


	//negamax search
	SearchResult negamax_search(Board& board, int depth, int alpha, int beta, int& node_count, MoveList list) {
		if (depth <= 0) {
			node_count += 1;
			std::cout << "\n-------------------------------------------\n";
			std::cout << "|\t\tQ_search_starts";
			std::cout << "\n-------------------------------------------\n";

			return quiescence(board, alpha, beta);

		}

		SearchResult result;
		//killer Move heuristic
		//int killer_nove[10][2];

		for (const Move& move : list) {
			board.make_move(move);
			//node_count += 1;

			int current_score = -Eval::get_evaluation(board);
			board.print_board();
			std::cout << "\n------------------------------------------------------------------\n";
			std::cout << "| Move: " << move_to_notation(move) << "\t";
			std::cout << "c_s: " << current_score << "\t";
			std::cout << "depth: " << depth << "\t";
			std::cout << "node: " << node_count << "\t";
			std::cout << "alpha: " << alpha << "\t";
			std::cout << "beta: " << beta << "|\t";
			std::cout << "\n------------------------------------------------------------------\n";
			if (current_score > alpha) alpha = current_score;


			if (current_score > beta ) {
				std::cout << "\n\t[beta_undo_move]"<< "\n";
				board.unmake_move(move);
				return SearchResult(current_score, move);
			}
			MoveList newlist;
			MoveGen::generate_legal_moves(board, newlist);
			move_ordering(board, newlist, Move() );


			SearchResult depth_result = negamax_search(board, depth - 1, -beta, -alpha, node_count, newlist);
			int score = -depth_result.score;

			board.unmake_move(move);
			if (result.score < score) {
				result.score = score;
				result.best_move = move;
			}
		}
		return result;
	}

	// quite search/Q-Search
	SearchResult quiescence(Board& board, int alpha, int beta) {
		SearchResult result;
		MoveList list;
		MoveGen::generate_legal_moves(board, list);
		result.score = Eval::get_evaluation(board);


		for (Move& move : list) {
			if (move.captured_piece != Pieces::EMPTY) {
				board.make_move(move);
				//
				int current_score = -Eval::get_evaluation(board);
				std::cout << "\nq-move: " << move_to_notation(move) << "\t";
				std::cout << "c_s: " << current_score << "\t";
				std::cout << "alpha: " << alpha << "\t";
				std::cout << "beta: " << beta << "\t";
				if (current_score >= beta) {
					board.unmake_move(move);
					std::cout << "\n\t[Q-beta_undo_move]" << "\n";
					return SearchResult(current_score, move);
				}
				if (current_score > alpha) alpha = current_score;
				//
				SearchResult depth_result = quiescence(board, -beta, -alpha);
				int score = -depth_result.score;
				board.unmake_move(move);
				if (result.score < score) {
					result.score = score;
					result.best_move = move;
				}
			}
		}
		std::cout << "\n-------------------------------------------\n";
		std::cout << "|\t\tQ_search_ends";
		std::cout << "\n-------------------------------------------\n";

		return result;

	}

} 