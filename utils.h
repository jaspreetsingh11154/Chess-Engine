#pragma once
#include <string>
#include "pieces.h"
//forward delecration
class Board;
struct Move;

std::string  square_to_notation(int square);
std::string  move_to_notation(const Move &move);
Move uci_to_move(std::string str, Board& board);
int notation_to_square(const std::string notation);
char piece_to_notation(Pieces piece);
Pieces notation_to_piece(char c);
