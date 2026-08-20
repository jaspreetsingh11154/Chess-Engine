#include "utils.h"
#include "board.h"
#include "MoveGen.h"

std::string square_to_notation(int square) {
	int rank = square / 10;
	int file = square % 10;
	std::string notation = "";
	notation += (char)'a' + (file-1);
	notation += (char)'1' + (9-rank);
	return notation;
}

std::string move_to_notation(const Move &move) {
	std::string notation = "";
	notation += square_to_notation(move.move_from);
	notation += square_to_notation(move.move_to);
	return notation;
}


int notation_to_square(const std:: string notation) {
	int square = 0;
	for (int i = 0; i <= 1; i++) {
		char c = notation[i];
		switch (c) {
		case'a': square += 1; continue;
		case'b': square += 2; continue;
		case'c': square += 3; continue;
		case'd': square += 4; continue;
		case'e': square += 5; continue;
		case'f': square += 6; continue;
		case'g': square += 7; continue;
		case'h': square += 8; continue;
		case'1': square += 90; continue;
		case'2': square += 80; continue;
		case'3': square += 70; continue;
		case'4': square += 60; continue;
		case'5': square += 50; continue;
		case'6': square += 40; continue;
		case'7': square += 30; continue;
		case'8': square += 20; continue;
		}
	}
	return square;
}

Move uci_to_move(std::string str, chess_board &board) {
	if (str.length() < 4) return Move{};

	char promotion_char = '\0';
	if (str.length() == 5) {
		promotion_char = str[4];
		//str = str.substr(0, 4);
	}
	int	move_from = notation_to_square(str.substr(0, 2));
	int move_to = notation_to_square(str.substr(2, 2));
	//using the legal move gen to automiticaly set the move flags
	MoveList move_list;
	MoveGen::generate_legal_moves(board, move_list);
	for (const Move& move : move_list) {
		if (move.move_from == move_from && move.move_to == move_to) {
			if (promotion_char != '\0') {
				if (promotion_char == piece_to_notation(move.promoted_piece)) return move;
			}
			else return move;
		}
	}
	return Move{};
}

char piece_to_notation(pieces piece) {
	const char array_lookup[] = { '.','P','N','B','R','Q','K','p','n','b','r','q','k','x' };
	return array_lookup[int(piece)];
}

pieces notation_to_piece(char c) {
	switch (c) {
				case 'P': return pieces::W_PAWN; 
				case 'R': return pieces::W_ROOK; 
				case 'B': return pieces::W_BISHOP; 
				case 'N': return pieces::W_KNIGHT; 
				case 'K': return pieces::W_KING; 
				case 'Q': return pieces::W_QUEEN; 
				case 'p': return pieces::B_PAWN; 
				case 'r': return pieces::B_ROOK; 
				case 'b': return pieces::B_BISHOP; 
				case 'n': return pieces::B_KNIGHT; 
				case 'q': return pieces::B_QUEEN; ;
				case 'k': return pieces::B_KING; 
				default: return pieces::EMPTY;
	}
}