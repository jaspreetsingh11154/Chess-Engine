#pragma once
#include "pieces.h"

enum class Moveflag {
	NONE, SHORT_CASTLE, LONG_CASTLE, EN_PASSANT 
};

struct Move {
	int move_from;
	int move_to;
	Pieces captured_piece;
	Pieces promoted_piece;
	Moveflag flag;
	//default constructor
	Move()
		:move_from(-1),
		move_to(-1),
		captured_piece(Pieces::EMPTY),
		promoted_piece(Pieces::EMPTY),
		flag(Moveflag::NONE) {}

	//constructer (member initializer list)->initialize the values first than allocate
	Move(int from, int to,
		Pieces captured_pieces = Pieces::EMPTY, 
		Pieces promoted_piece = Pieces::EMPTY, 
		Moveflag flag = Moveflag::NONE) 
		:move_from(from), 
		move_to(to), 
		captured_piece(captured_pieces), 
		promoted_piece(promoted_piece), 
		flag(flag) {}
	
	bool is_null() const {
		return move_from == -1 || move_to == -1;
	}

	bool operator==(const Move &other) const { 
		return move_from == other.move_from && move_to == other.move_to &&
			captured_piece == other.captured_piece && promoted_piece == other.promoted_piece; 
	}

};
