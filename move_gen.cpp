#include "move_gen.h"


//namespace with no name (anonymous namespace) is a private helper function for the file
namespace {
	void MoveGen_pawn(const Board& board, MoveList& list, const int square) {
		Pieces pawn = board.get_piece(square);
		Colours pawn_colour = get_piece_colour(pawn);
		int direction = pawn_colour == Colours::WHITE ? -10 : 10;
		int start_rank = pawn_colour == Colours::WHITE ? 8 : 3;
		int one_move_forward = square + direction;
		int promotion_rank = pawn_colour == Colours::WHITE ? 20 : 90;

		// 1 move forward promotion and move
		if (board.get_piece(one_move_forward) == Pieces::EMPTY) {
			//promotion 
			if (one_move_forward / 10 == promotion_rank / 10) {
				if (pawn_colour == Colours::WHITE) {
					for (Pieces promot_piece : {Pieces::W_BISHOP, Pieces::W_KNIGHT, Pieces::W_ROOK, Pieces::W_QUEEN}) {
						list.emplace_back(square, one_move_forward, Pieces::EMPTY, promot_piece);
					}
				}
				else if (pawn_colour == Colours::BLACK) {
					for (Pieces promot_piece : { Pieces::B_BISHOP, Pieces::B_KNIGHT, Pieces::B_ROOK, Pieces::B_QUEEN }) {
						list.emplace_back(square, one_move_forward, Pieces::EMPTY, promot_piece);
					}
				}
			}
			else {
				//1 move forward
				list.emplace_back(square, one_move_forward);

				// 2 move forward at start (inside the one move forward check statement)
				int two_move_forward = square + (direction * 2);
				if (square / 10 == start_rank && board.get_piece(two_move_forward) == Pieces::EMPTY) {
					list.emplace_back(square, two_move_forward);
				}
			}
		}
		// diagonal capture
		for (int offset : { direction - 1, direction + 1 }) {
			int target_square = square + offset;
			Pieces target_piece = board.get_piece(target_square);
			if (target_piece != Pieces::EMPTY && target_piece != Pieces::OFF_BOARD) {
				//enemy piece
				if (get_piece_colour(target_piece) != pawn_colour) {

					//capture promotion
					if (target_square / 10 == promotion_rank / 10) { 
						if (pawn_colour == Colours::WHITE) {
							for (Pieces promot_piece : {Pieces::W_BISHOP, Pieces::W_KNIGHT, Pieces::W_ROOK, Pieces::W_QUEEN}) {
								list.emplace_back(square, target_square, target_piece, promot_piece);
							}
						}
						else if (pawn_colour == Colours::BLACK) {
							for (Pieces promot_piece : { Pieces::B_BISHOP, Pieces::B_KNIGHT, Pieces::B_ROOK, Pieces::B_QUEEN }) {
								list.emplace_back(square, target_square, target_piece, promot_piece);
							}
						}
					}
					else list.emplace_back(square, target_square, target_piece);
				}
			}
			//in passing pawn( en_passant )
			if (target_square == board.get_en_passant_square()) {
				list.emplace_back(square, target_square, board.get_piece(target_square - direction), Pieces::EMPTY, Moveflag::EN_PASSANT);
			}
		}

	}

	void MoveGen_rook(const Board& board, MoveList& list, const int square) {
		int moves[4] = { 10,-10,1,-1 };
		Pieces rook = board.get_piece(square);
		Colours rook_colour = get_piece_colour(rook);

		for (int move : moves) {
			int target_square = square + move;
			Pieces target_piece = board.get_piece(target_square);
			while (target_piece != Pieces::OFF_BOARD) {
				//capture
				if (target_piece != Pieces::EMPTY) {
					if (get_piece_colour(target_piece) != rook_colour) list.emplace_back(square, target_square, target_piece);
					break;
				}
				else {
					list.emplace_back(square, target_square);
					target_square += move;
					target_piece = board.get_piece(target_square);
				}
			}
		}
	}
	void MoveGen_bishop(const Board& board, MoveList& list, const int square) {
		int moves[4] = { 11,-11,9,-9 };
		Pieces bishop = board.get_piece(square);
		Colours bishop_colour = get_piece_colour(bishop);

		for (int move : moves) {
			int target_square = square + move;
			Pieces target_piece = board.get_piece(target_square);
			while (target_piece != Pieces::OFF_BOARD) {
				//capture
				if (target_piece != Pieces::EMPTY) {
					if (get_piece_colour(target_piece) != bishop_colour)list.emplace_back(square, target_square, target_piece);
					break;
				}
				else {
					list.emplace_back(square, target_square);
					target_square += move;
					target_piece = board.get_piece(target_square);
				}
			}
		}
	}
	void MoveGen_knight(const Board& board, MoveList& list, const int square) {
		int moves[8] = { 21,19,12,-8,-19,-21,-12,8 };
		Pieces knight = board.get_piece(square);
		Colours knight_colour = get_piece_colour(knight);

		for (int move : moves) {
			int target_square = square + move;
			Pieces target_piece = board.get_piece(target_square);
			if (target_piece != Pieces::OFF_BOARD) {
				//capture
				if (target_piece != Pieces::EMPTY) {
					if (get_piece_colour(target_piece) != knight_colour)list.emplace_back(square, target_square, target_piece);
				}
				else {
					list.emplace_back(square, target_square);
				}
			}
		}
	}
	void MoveGen_king(const Board& board, MoveList& list, const int square) {
		int moves[8] = { 10,-10,1,-1 ,11,-11,9,-9 };
		Pieces king = board.get_piece(square);
		Colours king_colour = get_piece_colour(king);
		//casteling
		//int short_castle_square = square + 3;
		//int long_castle_square = square - 4;
		uint8_t short_castle_bit = king_colour == Colours::WHITE ? 1 : 4;
		uint8_t long_castle_bit = king_colour == Colours::WHITE ? 2 : 8;
		uint8_t castling_rights = board.get_castling_rights();
		if (!MoveGen::is_under_attack(board, square)) {
			//------------short_castle
			if (castling_rights & short_castle_bit) {
				if (board.get_piece(square + 1) == Pieces::EMPTY &&
					board.get_piece(square + 2) == Pieces::EMPTY) {
					if (!MoveGen::is_under_attack(board, square + 1) &&
						!MoveGen::is_under_attack(board, square + 2)) {
						list.emplace_back(square, square + 2, Pieces::EMPTY, Pieces::EMPTY, Moveflag::SHORT_CASTLE);
					}
				}
			}
			//----------------long_castle
			if (castling_rights & long_castle_bit) {
				if (board.get_piece(square - 1) == Pieces::EMPTY &&
					board.get_piece(square - 2) == Pieces::EMPTY &&
					board.get_piece(square - 3) == Pieces::EMPTY) {
					if (!MoveGen::is_under_attack(board, square - 1) &&
						!MoveGen::is_under_attack(board, square - 2) &&
						!MoveGen::is_under_attack(board, square - 2) ) {
						list.emplace_back(square, square - 2, Pieces::EMPTY, Pieces::EMPTY, Moveflag::LONG_CASTLE);
					}
				}
			}
		}
		
		//move
		for (int move : moves) {
			int target_square = square + move;
			Pieces target_piece = board.get_piece(target_square);
			if (target_piece != Pieces::OFF_BOARD) {
				//capture
				if (target_piece != Pieces::EMPTY) {
					if (get_piece_colour(target_piece) != king_colour)list.emplace_back(square, target_square,target_piece);
				}
				else {
					list.emplace_back(square, target_square);
				}
			}
		}
	}
	void MoveGen_queen(const Board& board, MoveList& list, const int square) {
		int moves[8] = { 10,-10,1,-1,11,-11,9,-9 };
		Pieces queen = board.get_piece(square);
		Colours queen_colour = get_piece_colour(queen);

		for (int move : moves) {
			int target_square = square + move;
			Pieces target_piece = board.get_piece(target_square);
			while (target_piece != Pieces::OFF_BOARD) {
				//capture
				if (target_piece != Pieces::EMPTY) {
					if (get_piece_colour(target_piece) != queen_colour) list.emplace_back(square, target_square, target_piece);
					break;
				}
				else {
					list.emplace_back(square, target_square);
					target_square += move;
					target_piece = board.get_piece(target_square);
				}
			}
		}
	}
	
}
//normal global namespace
namespace MoveGen {
	void generate_pseudo_legal_moves( const Board &board, MoveList& list ) {
		Colours move_side_colour = board.get_side_to_move();
		if (move_side_colour == Colours::BLACK) {
			for (int i = 20; i <= 90; i = i + 10) {
				for (int j = 1; j <= 8; j++) {
					int square = i + j;
					Pieces piece = board.get_piece(square);
					if (get_piece_colour(piece) == move_side_colour) {
						switch (piece) {
						case Pieces::B_PAWN:
							MoveGen_pawn(board, list, square);
							break;
						case Pieces::B_BISHOP:
							MoveGen_bishop(board, list, square);
							break;
						case Pieces::B_KNIGHT:
							MoveGen_knight(board, list, square);
							break;
						case Pieces::B_ROOK:
							MoveGen_rook(board, list, square);
							break;
						case Pieces::B_KING:
							MoveGen_king(board, list, square);
							break;
						case Pieces::B_QUEEN:
							MoveGen_queen(board, list, square);
							break;
						}

					}
				}
			}
		}
		else {
			for (int i = 90; i >= 20; i -= 10) {
				for (int j = 1; j <= 8; j++) {
					int square = i + j;
					Pieces piece = board.get_piece(square);
					if (get_piece_colour(piece) == move_side_colour) {
						switch (piece) {
						case Pieces::W_PAWN:
							MoveGen_pawn(board, list, square);
							break;
						case Pieces::W_BISHOP:
							MoveGen_bishop(board, list, square);
							break;
						case Pieces::W_KNIGHT:
							MoveGen_knight(board, list, square);
							break;
						case Pieces::W_ROOK:
							MoveGen_rook(board, list, square);
							break;
						case Pieces::W_KING:
							MoveGen_king(board, list, square);
							break;
						case Pieces::W_QUEEN:
							MoveGen_queen(board, list, square);
							break;
						}

					}
				}
			}
		}
		
	}

	bool MoveGen::is_under_attack(const Board& board, int square) {
		Pieces piece = board.get_piece(square);
		Colours piece_colour{};
		if (piece != Pieces::EMPTY) piece_colour = get_piece_colour(piece);
		else piece_colour = board.get_side_to_move();

		Pieces enemy_king = piece_colour == Colours::WHITE ? Pieces::B_KING : Pieces::W_KING;
		Pieces enemy_queen = piece_colour == Colours::WHITE ? Pieces::B_QUEEN : Pieces::W_QUEEN;
		Pieces enemy_rook = piece_colour == Colours::WHITE ? Pieces::B_ROOK : Pieces::W_ROOK;
		Pieces enemy_bishop = piece_colour == Colours::WHITE ? Pieces::B_BISHOP : Pieces::W_BISHOP;
		Pieces enemy_pawn = piece_colour == Colours::WHITE ? Pieces::B_PAWN : Pieces::W_PAWN;
		Pieces enemy_knight = piece_colour == Colours::WHITE ? Pieces::B_KNIGHT : Pieces::W_KNIGHT;
		for (int N_square : {21, 19, 12, -8, -19, -21, -12, 8 }) {
			Pieces target = board.get_piece(square + N_square);
			if (target == enemy_knight) {
				return true;
			}
		}
		//pawn attack check
		int pawn_direction = piece_colour == Colours::WHITE ? -10 : 10;
		if (board.get_piece(square + pawn_direction - 1) == enemy_pawn) {
			return true;
		}
		if (board.get_piece(square + pawn_direction + 1) == enemy_pawn) {
			return true;
		}
		//loop 
		int moves[8] = { 10,-10,11,-11,9,-9,1,-1 };
		for (int move : moves) {
			int target_square = square + move;
			Pieces target_piece = board.get_piece(target_square);
			if (target_piece == enemy_king) return true;
			while (target_piece != Pieces::OFF_BOARD) {
				if (target_piece != Pieces::EMPTY) {
					if (piece_colour != get_piece_colour(target_piece)) {
						switch (move) {
						case 10:
						case -10:
						case 1:
						case -1:
							if (target_piece == enemy_queen || target_piece == enemy_rook) {
								return true;
							}
							break;
						case 11:
						case -11:
						case 9:
						case -9:
							if (target_piece == enemy_queen || target_piece == enemy_bishop) {
								return true;
							}
							break;
						}
						//CRITICAL: breaks the while loop if the enmey pices can not attack
						break;
					}
					else {
						//CRITICAL: breake the while loop for same coloured piece
						break;
					}
				}
				else {
					target_square += move;
					target_piece = board.get_piece(target_square);
				}
			}
		}
		return false;
	}

	void generate_legal_moves(Board& board, MoveList &legal_move_list) {
		MoveList raw_move_list;
		raw_move_list.clear();
		generate_pseudo_legal_moves(board, raw_move_list);
		for (const Move& move : raw_move_list) {
			Colours king_colour = get_piece_colour(board.get_piece(move.move_from));
			board.make_move(move);
			//CRITICAL:->checking if king is  under attack 
			if (!MoveGen::is_under_attack(board, board.get_king_square(king_colour))) {
				legal_move_list.push_back(move);
			}
			board.unmake_move(move);
		}
	}

}