// Chess.cpp : Defines the entry point for the application.
//

#include <iostream>
#include "MoveGen.h"
#include "perft.h"
#include <sstream>
#include "Evaluation.h"
#include "search.h"

int main()
{
	chess_board board;
	//board.load_fen("r3k2r/pppppppp/8/8/8/8/PPPPPPP1/R3K2R w KQkq - 1 1");
	board.print_board();
	search::SearchResult result;
	int node_count = 0;
	result = search::negamax_search(board, 3, -1000000, 1000000, node_count);
	std::cout << "\nnode: " << node_count << std::endl;
	std::cout << move_to_notation(result.best_move) << ": " << result.score;
	std:: cout << "\nEval: " << Eval::get_evaluation(board);
	return 0;

	std::string command;
	board.load_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 1 1");
	std::cout << "\ncastling rights->" <<(int)board.get_castling_rights() << "\n";
	////board.load_fen("8/3p4/8/4P3/8/8/8/8 b - -");
	std::cout << "en_passant_square->" << square_to_notation(board.get_en_passant_square()) << "\n";
	std::cout << "halfmove->" << (int)board.get_halfmove_clock() << "\n";
	board.print_board();
	////uint64_t node1 = perft(board, 2);
	//uint64_t node2 = perft_divide(board, 4);
	////std::cout << "nodes1: " << node1;
	//std::cout << "nodes2: " << node2;
	return 0;
	//******IMP*** endl is nessery for fulshin the buffer
	while (std::getline(std::cin, command)) {
		if (command == "uci") {
			std::cout << "id name MyEngine" << std::endl;
			std::cout << "uciok" << std::endl;
		
		}
		else if (command == "isready") {
			std::cout << "readyok" << std::endl;
		}
		else if (command == "quit") {
			break;
		}
		else if (command.rfind("position",0) == 0) {
			if (command.rfind("startpos") != std::string::npos) {
				board.reset_board();
			}
			else {
				std::size_t fen_idx = command.find("fen");
				std::size_t move_idx = command.rfind("move");
				if (fen_idx != std::string::npos) {
					std::string fen_str = command.substr(fen_idx + 4); 
					if (move_idx == std::string::npos) {
						board.load_fen(fen_str);
					}
					else {
						fen_str = fen_str.substr(0, move_idx - 1);
						//board.load_fen(fen_str);
						std::string move_str = command.substr(move_idx + 5);
						std::stringstream ss(move_str);
						std::string token;
						while (ss >> token) {
							Move move = uci_to_move(token, board);
							if (!move.is_null()) board.Make_move(move);
						}
					}
				}
			}
		}
		else if (command.rfind("go perft", 0) == 0) {
			std::stringstream ss(command);
			std::string sub1, sub2;
			int depth = 1; //std::stoi(command.substr(9));
			ss >> sub1 >> sub2 >> depth;
			uint64_t nodes = perft_divide(board, depth);

			std::cout << "\nNodes searched: " << nodes << std::endl;

		}
		else if (command.rfind("print board", 0) == 0) {
			board.print_board();
			std:: cout << std::endl;
		}
	}
	//std::cout << "Hello Chess." << std::endl;

	//random position
	//board.load_fen("8/3Pp3/8/8/8/8/8/8 w - - 0 1");
	//std::vector<Move> list;
	//MoveGen::generate_pseudo_legal_moves(board, list);
	//for ( Move m : list) {
	//	std::cout << square_to_notation(m.move_to)<< " ";
	//}
	//std::cout << "<-move-TO\n";


	return 0;
}
