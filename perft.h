#pragma once
#include <cstdint>

//forward decelaration
class Board;

uint64_t perft(Board& borad, int depth);
uint64_t perft_divide(Board& board, int depth);