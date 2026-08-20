#pragma once
#include <cstdint>

//forward decelaration
class chess_board;

uint64_t perft(chess_board& borad, int depth);
uint64_t perft_divide(chess_board& board, int depth);