// tictactoe.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <assert.h>
#include <iomanip>
enum class Cell : std::uint8_t {
	Empty = 0,
	Player_A = 1,
	Player_B = 2,
};
enum class Winner : std::uint8_t {
	Still_playing = 0,
	Player_A = 1,
	Player_B = 2,
	Tie = 3,

};
enum class Player : std::uint8_t {
	Player_A = 1,
	Player_B = 2,
};
struct Grid {
	size_t rows_;
	size_t cols_;
	Cell *cells_;
};
struct Vec2 {
	size_t x;
	size_t y;
};

Grid grid_init(size_t rows, size_t cols) {
	size_t cell_count = rows * cols;
	auto memory = static_cast<Cell*>(calloc(cell_count, sizeof(Cell)));
	assert(memory);
	Grid grid = {
		.rows_ = rows,
		.cols_ = cols,
		.cells_ = memory,

	};
	return grid;
}
void grid_free(Grid* grid) {
	free(grid->cells_);
}
Cell* get_cell(Grid* grid, size_t x_pos, size_t y_pos) {
	assert(x_pos < grid->cols_);
	assert(y_pos < grid->rows_);
	return &grid->cells_[x_pos + y_pos * grid->cols_];
}

void print_grid(Grid* grid) {
	for (size_t i = 0; i < grid->cols_; ++i) {
		for (size_t j = 0; j < grid->rows_; ++j) {
			auto cell = *get_cell(grid, i, j);
			auto character =
				cell == Cell::Empty ? '.' :
				cell == Cell::Player_A ? 'X' : 'O';
			std::cout << character;
		}
		std::cout << std::endl;
	}
}

Winner check_Winner(Grid* grid) {


	for (size_t x = 0; x < grid->cols_; ++x) {
		auto first_value = *get_cell(grid, x, 0);
		if (first_value == Cell::Empty) { continue; }
		for (size_t y = 1; y < grid->rows_; ++y) {
			if (first_value != *get_cell(grid, x, y)) {
				goto values_are_different_vertical;
			}
		}
		return static_cast<Winner>(first_value);
	values_are_different_vertical:;
	}
	for (size_t y = 0; y < grid->rows_; ++y) {
		auto first_value = *get_cell(grid, 0, y);
		if (first_value == Cell::Empty) { continue; }
		for (size_t x = 1; x < grid->cols_; ++x) {
			if (first_value != *get_cell(grid, x, y)) {
				goto values_are_different_horizontal;
			}
		}
		return static_cast<Winner>(first_value);
	values_are_different_horizontal:;
	}
	auto first_value_diagonal_left = *get_cell(grid, 0, 0);
	if (first_value_diagonal_left == *get_cell(grid, 1, 1) && first_value_diagonal_left == *get_cell(grid, 2, 2)) { return static_cast<Winner>(first_value_diagonal_left); }
	
	auto first_value_diagonal_right = *get_cell(grid, 2, 0);
	if (first_value_diagonal_right == *get_cell(grid, 1, 1) && first_value_diagonal_right == *get_cell(grid, 0, 2)) { return static_cast<Winner>(first_value_diagonal_right); }
	
	size_t possible_full_cells = grid->rows_ * grid->cols_;
	size_t full_cells = 0;
	for (int i = 0; i < grid->cols_; ++i) {
		for (int j = 0; j < grid->rows_; ++j) {
			if (*get_cell(grid, i, j) != Cell::Empty) {
				full_cells++;
			}
		}
	}
	if (full_cells == possible_full_cells) return Winner::Tie;
	return Winner::Still_playing;
}
int minimax(Grid* grid, bool isMaximising) {
	Winner result = check_Winner(grid);

	if (result != Winner::Still_playing) {
		switch (result) {
		case Winner::Player_A: return -1;
			break;
		case Winner::Player_B: return 1;
			break;
		case Winner::Tie: return 0;
			break;
		}
	}
	if (isMaximising) {
		int bestScore = -5; //random big number
		for (int i = 0; i < grid->cols_; ++i) {
			for (int j = 0; j < grid->rows_; ++j) {
				if (*get_cell(grid, i, j) == Cell::Empty) {
					*get_cell(grid, i, j) = Cell::Player_B;
					int score = minimax(grid, false);
					*get_cell(grid, i, j) = Cell::Empty;
					if (score > bestScore) {
						bestScore = score;
					}
				}
			}
		}
		return bestScore;
	}
	else {
		int bestScore = 5; //random big number
		for (int i = 0; i < grid->cols_; ++i) {
			for (int j = 0; j < grid->rows_; ++j) {
				if (*get_cell(grid, i, j) == Cell::Empty) {
					*get_cell(grid, i, j) = Cell::Player_A;
					int score = minimax(grid, true);
					*get_cell(grid, i, j) = Cell::Empty;
					if (score < bestScore) {
						bestScore = score;
					}
				}
			}
		}
		return bestScore;
	}
}
Vec2 best_move_ai(Grid* grid) {
	int best_score = -500; //random big number
	Vec2 best_move = {2, 2};
	for (size_t j = 0; j < grid->rows_; ++j) {
		for (size_t i = 0; i < grid->cols_; ++i) {
			if (*get_cell(grid, i, j) == Cell::Empty) {
				*get_cell(grid, i, j) = Cell::Player_B;
				int score = minimax(grid, false);
				//std::cout << std::right << std::setw(4) << score;
				
				*get_cell(grid, i, j) = Cell::Empty;
				if (score > best_score) {
					best_score = score;
					best_move = { i,j };
				}
			}
			//else { 
			//	std::cout << "   x";
			//}
		}
		std::cout << std::endl;
	}
	return best_move;
}
int main()
{
	size_t rows, cols;
	while (true) {
		std::cout << "How many rows (Minimum 3)" << std::endl;
		int test_rows, test_cols;
		std::cin >> test_rows;
		std::cout << "How many cols (Minimum 3)" << std::endl;
		std::cin >> test_cols;
		if (test_cols >= 3 && test_rows >= 3 ) {
			cols = test_cols;
			rows = test_rows;
			break;
		}
		std::cout << "Enter again" << std::endl;
	}
	
	Grid board = grid_init(rows, cols);
	bool game_won = false;
	char symbol = 'A';
	char symbol1 = 'B';
	Player current_player = Player::Player_A;

	while (!game_won)
	{
		print_grid(&board);
		Winner result = check_Winner(&board);
		switch (result) {
		case Winner::Player_A:
			std::cout << "Player_A has won" << std::endl;
			game_won = true;
			goto game_finished;
			break;
		case Winner::Player_B:
			std::cout << "Player_B has won" << std::endl;
			game_won = true;
			goto game_finished;
			break;
		case Winner::Tie:;
			std::cout << "It's a tie" << std::endl;
			game_won = true;
			goto game_finished;
			break;
		case Winner::Still_playing:
			std::cout << "Next round" << std::endl;
			break;
		}
		if (current_player == Player::Player_A) {
			while (true) {
				std::cout << "Player A enter your turn[First input][Second Input]" << std::endl;
				size_t x_pos = 0;
				size_t y_pos = 0;
				std::cin >> x_pos >> y_pos;
				x_pos--;
				y_pos--; // to adjust for 0 index
				if (x_pos >= cols || x_pos < 0 || y_pos >= rows || y_pos < 0) {
					std::cout << "Enter again" << std::endl;
					continue;
				}
				if (*get_cell(&board, x_pos, y_pos) != Cell::Empty) {
					std::cout << "Already filled" << std::endl;
					continue;
				}
				*get_cell(&board, x_pos, y_pos) = Cell::Player_A;
				break;
			}
			current_player = Player::Player_B;
			continue;
		}
		if (current_player == Player::Player_B) { 
			Vec2 vector2 = best_move_ai(&board);
			*get_cell(&board, vector2.x, vector2.y) = Cell::Player_B;
			current_player = Player::Player_A;
		}

	}
	game_finished: grid_free(&board);
	
}




// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
