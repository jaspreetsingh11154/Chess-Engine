#pragma once
#include <string>
#include "pieces.h"
//forward delecration
class chess_board;
struct Move;

std::string  square_to_notation(int square);
std::string  move_to_notation(const Move &move);
Move uci_to_move(std::string str, chess_board& board);
int notation_to_square(const std::string notation);
char piece_to_notation(pieces piece);
pieces notation_to_piece(char c);
