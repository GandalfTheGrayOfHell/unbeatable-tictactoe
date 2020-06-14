#include <stdio.h>
#include <stdbool.h>

#define max(a, b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })
#define min(a, b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })

typedef enum Chars {
	E = ' ',
	X = 'X',
	O = 'O',
} Chars;

typedef struct Move {
	int row, col;
} Move;

typedef struct Board {
	Chars board[3][3];
	bool win;
} Board;

Chars player = O, opponent = X;

void board_init(Board * b) {
	b->board[0][0] = E;
	b->board[0][1] = E;
	b->board[0][2] = E;
	b->board[1][0] = E;
	b->board[1][1] = E;
	b->board[1][2] = E;
	b->board[2][0] = E;
	b->board[2][1] = E;
	b->board[2][2] = E;

	b->win = false;
}

bool moves_left(Board * b) {
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			if (b->board[i][j] == E)
				return true;

	return false;
}

void draw(Board * b) {
	printf("========\n");
	printf("%c|%c|%c\n", b->board[0][0], b->board[0][1], b->board[0][2]);
	printf("%c|%c|%c\n", b->board[1][0], b->board[1][1], b->board[1][2]);
	printf("%c|%c|%c\n", b->board[2][0], b->board[2][1], b->board[2][2]);
}

bool check_input(int i, int j) {
	if (i > 3 || j > 3 || i < 0 || j < 0) return false;
	return true;
}

bool check_empty(Board * b, int i, int j) {
	if (b->board[i][j] == E) return true;
	else return false; 
}

int evaluate(Board * b) {
	for (int row = 0; row < 3; ++row) {
		if (b->board[row][0] == b->board[row][1] && b->board[row][1] == b->board[row][2]) {
			if (b->board[row][0] == player) return +10;
			else if (b->board[row][0] == opponent) return -10;
		}
	}

	for (int col = 0; col < 3; ++col) {
		if (b->board[0][col] == b->board[1][col] && b->board[1][col] == b->board[2][col]) {
			if (b->board[0][col] == player) return +10;
			else if (b->board[0][col] == opponent) return -10;
		}
	} 
 
	if (b->board[0][0] == b->board[1][1] && b->board[1][1] == b->board[2][2]) {
		if (b->board[0][0] == player) return +10;
		else if (b->board[0][0] == opponent) return -10;
	}

	if (b->board[0][2] == b->board[1][1] && b->board[1][1] == b->board[2][0]) {
		if (b->board[0][2] == player) return +10;
		else if (b->board[0][2] == opponent) return -10;
	}

	return 0;
}

int minimax(Board * b, int depth, bool isMax) {
	int score = evaluate(b);

	if (score == 10) return score;

	if (score == -10) return score;

	if (moves_left(b) == false) return 0;

	if (isMax) {
		int best = -1000;

		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				if (b->board[i][j] == E) {
					b->board[i][j] = player;

					best = max(best, minimax(b, depth + 1, !isMax));

					b->board[i][j] = E;
				}
			}
		}

		return best;
	}
	else {
		int best = 1000;
		
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				if (b->board[i][j] == E) {
					b->board[i][j] = opponent;

					best = min(best, minimax(b, depth + 1, !isMax));

					b->board[i][j] = E;
				}
			}
		}

		return best;
	}
}

Move find_best_move(Board * b) {
	int best_val = -1000;
	
	Move best_move;
	best_move.row = -1;
	best_move.col = -1;

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (b->board[i][j] == E) {
				b->board[i][j] = player;
 
				int val = minimax(b, 0, false);

				b->board[i][j] = E;

				if (val > best_val) {
					best_move.row = i;
					best_move.col = j;
					best_val = val;
				}
			}
		}
	}

	return best_move;
}

int main() {
	Board board;
	Board * b = &board;

	board_init(b);

	int end = 1;
	while(end) {
		draw(b);
		if (moves_left(b) == false) end = 0;

		int win_check = evaluate(b);
		if (win_check == 10) {
			printf("%c Won!", player);
			end = 0;
			continue;
		}
		else if (win_check == -10) {
			printf("%c", opponent);
			end = 0;
			continue;
		}

		int row, col;

		printf("\n");
		printf("Enter row (1-3): ");
		scanf("%d", &row);
		printf("Enter col (1-3): ");
		scanf("%d", &col);

		if (!check_input(row - 1, col - 1)) {
			printf("Please enter valid input.\n");
			continue;
		}
		
		if (!check_empty(b, row - 1, col - 1)) {
			printf("Position already occupied. Try another position.\n");
			continue;
		}

		b->board[row - 1][col - 1] = opponent;

		Move bestMove = find_best_move(b); 

		b->board[bestMove.row][bestMove.col] = player;
	}

	return 0; 
}