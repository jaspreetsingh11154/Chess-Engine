#include "board.h"
#include <iostream>
#include <sstream>

//constructer 
Board::Board() {
	reset_board();
}
void Board::print_board() const {
	std::cout << "\n<----[Board]---->" << std::endl;
	int row = 8;
	for (int i = 20; i <= 90; i = i + 10) {
		std::cout << row << "|";
		for (int j = 1; j <= 8; j++) {
			//using char type enum class
			//std::cout << (char)position_board[i + j] << " ";
			//using int type enum class
			std::cout << piece_to_notation(position_board[i + j]) << " ";
		}
		std::cout << std::endl;
		row--;
	}
	for (char c : { '0', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'}) {
		std::cout << c << " ";
	}
	std::cout << "\n<--board printed-->\n";

}

void Board::clear_board() {
	//initialise the board with off board
	std::fill(std::begin(position_board), std::end(position_board), Pieces::OFF_BOARD);
	//initialiseing the playing 8x8 board with empty squars
	for (int i = 20; i <= 90; i = i + 10) {
		for (int j = 1; j <= 8; j++) {
			position_board[i + j] = Pieces::EMPTY;
		}
	}
}

void Board::reset_board() {
	clear_board();
	//side to move
	side_to_move = Colours::WHITE;
	//initialising the king tracking 
	white_king_square = 95;
	black_king_square = 25;
	//initialising other variables
	en_passant_square = -1;
	castling_rights = 15;
	history_stack.clear();
	halfmove_clock = 0;
	//pieces placement
	//black
	position_board[21] = Pieces::B_ROOK; 
	position_board[22] = Pieces::B_KNIGHT;
	position_board[23] = Pieces::B_BISHOP;
	position_board[24] = Pieces::B_QUEEN;
	position_board[25] = Pieces::B_KING;
	position_board[26] = Pieces::B_BISHOP;
	position_board[27] = Pieces::B_KNIGHT;
	position_board[28] = Pieces::B_ROOK;
	for (int i = 31; i < 39; i++) {
		position_board[i] = Pieces::B_PAWN;
	}
	 
	//white
	position_board[91] = Pieces::W_ROOK;
	position_board[92] = Pieces::W_KNIGHT;
	position_board[93] = Pieces::W_BISHOP;
	position_board[94] = Pieces::W_QUEEN;
	position_board[95] = Pieces::W_KING;
	position_board[96] = Pieces::W_BISHOP;
	position_board[97] = Pieces::W_KNIGHT;
	position_board[98] = Pieces::W_ROOK;
	for (int i = 81; i < 89; i++) {
		position_board[i] = Pieces::W_PAWN;
	}
 
}
//"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
void Board::load_fen(const std::string& fen) {

	clear_board();
	int rank = 20;
	int file = 1;
	int i = 0;
	//
	std::stringstream ss(fen);
	std::string fen_part1, turn, castel_rights, en_passant, hm1, hm2;
	ss >> fen_part1 >> turn >> castel_rights >> en_passant >> hm1 >> hm2;
	for (char c : fen_part1) {
		if (c == '/') {
			rank += 10;
			file = 1;
			continue;
		}
		else if (isdigit(c)) file += (c - '0') - 1;
		else {
			Pieces piece = notation_to_piece(c);
			position_board[rank + file] = piece;
			if (piece == Pieces::B_KING) black_king_square = rank + file;
			if (piece == Pieces::W_KING) white_king_square = rank + file;
		}
		file++;
	}
	//sdie to move
	this->side_to_move = turn[0] == 'w' ? Colours::WHITE : Colours::BLACK;
	//casteling
	for (char c : castel_rights) {
		switch (c) {
			case'-': continue;
			case'K': this->castling_rights |= 1; break;
			case'Q': this->castling_rights |= 2; break;
			case'k': this->castling_rights |= 4; break;
			case'q': this->castling_rights |= 8; break;
		}
	}
	//en_passant
	if (en_passant[0] != '-') this->en_passant_square = notation_to_square(en_passant);
	//halfmove 

	this->halfmove_clock = std::stoi(hm1) + std::stoi(hm2);
	return;
}


void Board::make_move(const Move& move) {
	//push the state into history 
	history_stack.emplace_back(this->castling_rights, this->en_passant_square, this->halfmove_clock);
	//increment hm clock if piece is captured hm = 0, or pawn is moved hm =0
	if (move.captured_piece != Pieces::EMPTY) halfmove_clock = 0;
	else halfmove_clock++; 
	

	//-----storing the moving piece
	Pieces move_piece = get_piece(move.move_from);
	Colours piece_colour = get_piece_colour(move_piece);
	//std::cout << " Make move->\n";
	//std::cout << " piece->" << piece_to_notation(move_piece);
	//std::cout << " move_from->" << square_to_notation(move.move_from) << "\n";
	//std::cout << " move_to->" << square_to_notation(move.move_to) << "\n";

	//-----clear the previous position
	position_board[move.move_from] = Pieces::EMPTY;
	//----removing the en_passant_square
	en_passant_square = 0;
	//---------------------update the piece on new poisition----------------
	//promotion
	if (move.promoted_piece != Pieces::EMPTY) 
		position_board[move.move_to] = move.promoted_piece; 
	else 
		position_board[move.move_to] = move_piece; 
	//checking en_passing_square
	if (move_piece == Pieces::W_PAWN || move_piece == Pieces::B_PAWN) {
		//pawn is moved hm = 0
		halfmove_clock = 0;
		//-------------set the en_passant square on pawn 2 square move---------------
		int pawn_direction = piece_colour == Colours::WHITE ? -10 : 10;
		if (move.move_to - move.move_from == 2 * pawn_direction) {
			en_passant_square = move.move_from + pawn_direction;
		}
	}
	switch (move.flag) {
	case Moveflag::EN_PASSANT: {
		int behind_pawn = piece_colour == Colours::WHITE ? -10 : 10;
		//empty the captured pawn which is behind the en_passant 
		position_board[move.move_to - behind_pawn] = Pieces::EMPTY;
		break;
	}
	case Moveflag::LONG_CASTLE: {
		int move_rook_from = piece_colour == Colours::WHITE ? 91 : 21;
		Pieces rook = get_piece(move_rook_from);
		position_board[move_rook_from] = Pieces::EMPTY;
		position_board[move_rook_from + 3] = rook;
		break;
	}
	case Moveflag::SHORT_CASTLE: {
		int move_rook_from = piece_colour == Colours::WHITE ? 98 : 28;
		Pieces rook = get_piece(move_rook_from);
		position_board[move_rook_from] = Pieces::EMPTY;
		position_board[move_rook_from - 2] = rook;
		break;
	}

	}
	//changing castling rights using castling mask
	castling_rights &= get_castling_mask(move.move_from);
	castling_rights &= get_castling_mask(move.move_to);

	//update king tracking after king moved
	if (move_piece == Pieces::W_KING) {
		white_king_square = move.move_to;
	}
	else if (move_piece == Pieces::B_KING) {
		black_king_square = move.move_to;
	}
	//switch side to move after the move 
	if (side_to_move == Colours::WHITE) side_to_move = Colours::BLACK;
	else if(side_to_move == Colours::BLACK) side_to_move = Colours::WHITE;
	//if (position_board[78]==pieces::B_QUEEN)
	//	std::cout << "\nError in Make move during move.from->" << square_to_notation(move.move_from) << "move.to->" << square_to_notation(move.move_to) << "\n";

}

void Board::unmake_move(const Move& move) {
	Pieces  move_piece_back = get_piece(move.move_to);
	Colours piece_colour = get_piece_colour(move_piece_back);
	//std::cout << "\nUnake move->\n";
	//std::cout << "piece->" << piece_to_notation(move_piece_back);
	//std::cout << "move_from->" << square_to_notation(move.move_from) << "\n";
	//std::cout << "move_to->" << square_to_notation(move.move_to) << "\n";
	//clear the move_to position
	position_board[move.move_to] = Pieces::EMPTY;
	//----------------------[unmake capturing move]---------------------
	//unmake captured piece moves
	Pieces captured_piece = move.captured_piece;
	if (captured_piece != Pieces::EMPTY) {
		//unmake en_passant capture
		if (move.flag == Moveflag::EN_PASSANT) {
			int direction = piece_colour == Colours::WHITE ? -10 : 10;
			//put the captured pawn back which is behind the en_passant 
			position_board[move.move_to - direction] = captured_piece;
			position_board[move.move_from] = move_piece_back;
		}
		//unmake promotion capture
		else if (move.promoted_piece != Pieces::EMPTY) {
			//put the captured piece back
			position_board[move.move_to] = captured_piece;
			//put the capturing pawn piece back
			position_board[move.move_from] = piece_colour == Colours::WHITE ? Pieces::W_PAWN : Pieces::B_PAWN ;
		}
		//unmake normal captures
		else {
			position_board[move.move_to] = captured_piece;
			position_board[move.move_from] = move_piece_back;
		}
	}
	//---------------------[unmake the non_capturing moves]---------------------
	else {
		//unmkae non capturing normal moves
		position_board[move.move_from] = move_piece_back;
		//pawn farward promotion
		if (move.promoted_piece != Pieces::EMPTY) {
			position_board[move.move_from] = piece_colour == Colours::WHITE ? Pieces::W_PAWN : Pieces::B_PAWN;
		}
		//catling
		switch (move.flag) {
		case Moveflag::LONG_CASTLE: {
			int move_rook_back = piece_colour == Colours::WHITE ? 94 : 24;
			Pieces rook = get_piece(move_rook_back);
			position_board[move_rook_back] = Pieces::EMPTY;
			position_board[move_rook_back - 3] = rook;
			break;
		}
		case Moveflag::SHORT_CASTLE: {
			int move_rook_back = piece_colour == Colours::WHITE ? 96 : 26;
			Pieces rook = get_piece(move_rook_back);
			position_board[move_rook_back] = Pieces::EMPTY;
			position_board[move_rook_back + 2] = rook;
			break;
		}
		}
	}
	//------------------------[unmake the last move state]----------------
	UndoState& last_state = history_stack.back();
	this->castling_rights = last_state.castling_rights;
	this->en_passant_square = last_state.en_passant_square;
	this->halfmove_clock = last_state.halfmove_clock;
	history_stack.pop_back();
	//update king tracking after king moved
	if (move_piece_back == Pieces::W_KING) {
		white_king_square = move.move_from;
	}
	else if (move_piece_back == Pieces::B_KING) {
		black_king_square = move.move_from;
	}
	//switch side to move after the move 
	if (side_to_move == Colours::WHITE) side_to_move = Colours::BLACK;
	else if (side_to_move == Colours::BLACK) side_to_move = Colours::WHITE;

	//-------------
	//if (position_board[78] == pieces::B_QUEEN)
	//	std::cout << "\nError in Unmake during move.from->" << square_to_notation(move.move_from) << "move.to->" << square_to_notation(move.move_to) << "\n";

}
