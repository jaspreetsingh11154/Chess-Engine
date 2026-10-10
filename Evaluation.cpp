#include "Evaluation.h"
#include <iostream>
namespace Eval {

int get_evaluation(const Board& board) {
	int white_score = 0;
	int black_score = 0;
	for (int i = 20; i <= 90; i += 10) {
		for (int j = 1; j <= 8; j++) {
			Pieces piece = board.get_piece(i+j);
			if (piece == Pieces::OFF_BOARD || piece == Pieces::EMPTY) continue;
			int index = (int)piece - 1;
			if (index >= 6) {
				index -= 6;
				black_score += get_pst_score(piece , i + j) + piece_value[index];
			}
			else {
				white_score += get_pst_score(piece, i + j) + piece_value[index];

			}
		}
	}
	//std::cout << "\nW_s: " << white_score;
	//std::cout << "\nb_s: " << black_score << "\n";

	if (board.get_side_to_move() == Colours::BLACK)
		return (black_score - white_score); 
	else
		return (white_score - black_score);
}

}
	