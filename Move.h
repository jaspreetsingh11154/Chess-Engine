#pragma once
#include "pieces.h"

enum class Moveflag {
	NONE, SHORT_CASTLE, LONG_CASTLE, EN_PASSANT 
};

struct Move {
	int move_from;
	int move_to;
	pieces captured_piece;
	pieces promoted_piece;
	Moveflag flag;
	//default constructor
	Move()
		:move_from(-1),
		move_to(-1),
		captured_piece(pieces::EMPTY),
		promoted_piece(pieces::EMPTY),
		flag(Moveflag::NONE) {}

	//constructer (member initializer list)->initialize the values first than allocate
	Move(int from, int to,
		pieces captured_pieces = pieces::EMPTY, 
		pieces promoted_piece = pieces::EMPTY, 
		Moveflag flag = Moveflag::NONE) 
		:move_from(from), 
		move_to(to), 
		captured_piece(captured_pieces), 
		promoted_piece(promoted_piece), 
		flag(flag) {}
	
	bool is_null() const {
		return move_from == -1 || move_to == -1;
	}
};
