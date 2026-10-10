#pragma once

enum class Colours { WHITE, BLACK, NONE };
enum class Pieces : int {
	EMPTY = 0, W_PAWN = 1, W_KNIGHT = 2, W_BISHOP = 3, W_ROOK = 4, W_QUEEN = 5, W_KING = 6,
	B_PAWN = 7, B_KNIGHT = 8, B_BISHOP = 9, B_ROOK = 10, B_QUEEN = 11, B_KING = 12, OFF_BOARD = 13
};
//inline will compile the function fast without jumps as it will copy it every where it is called
inline Colours get_piece_colour(Pieces piece) {
	if (piece >= Pieces::W_PAWN && piece <= Pieces::W_KING) return Colours::WHITE;
	if (piece >= Pieces::B_PAWN && piece <= Pieces::B_KING) return Colours::BLACK;
	return Colours::NONE;
}

//enum class pieces : char {
//	EMPTY = '-', W_PAWN = 'p', W_KNIGHT = 'n', W_BISHOP = 'b', W_ROOK = 'r', W_QUEEN = 'q', W_KING = 'k',
//	B_PAWN = 'P', B_KNIGHT = 'N', B_BISHOP = 'B', B_ROOK = 'R', B_QUEEN = 'Q', B_KING = 'K', OFF_BOARD = 'x'
//
//};