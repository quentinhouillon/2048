// ldd: -lncurses
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>

typedef enum {
	up = KEY_UP,
	down = KEY_DOWN,
	left = KEY_LEFT,
	right = KEY_RIGHT,
	backspace = KEY_BACKSPACE,
} Key;

#define TabSize 4
int board[TabSize][TabSize];
unsigned int score = 0;

void INIT_2048() {
	initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();
	srand(time(NULL));
}

void DONE_2048() {
	endwin();
	exit(0);
}

void init_board() {
	for (unsigned int i=0; i<TabSize; i++) {
		for (unsigned int j=0; j<TabSize; j++) {
			board[i][j] = 0;
		}
	}
}

void display_board() {
	move(0, 0);
	printw("== 2048 =============================\n");
	printw("======================= score :%5i\n", score);
	for (unsigned int i=0; i<TabSize; i++) {
		printw("+ - - - - - - - - + - - - - - - - - + - - - - - - - - + - - - - - - - - +\n");
		printw("| %17 | %17 | %17 | %17 |\n");
		printw("|");
		for (unsigned int j=0; j<TabSize; j++) {
			if (board[i][j]) {
				printw("%9i%9 |", board[i][j]);
			}
			else {
				printw("%18|");
			}
		}
		printw("\n| %17 | %17 | %17 | %17 |\n");
	}
	printw("+ - - - - - - - - + - - - - - - - - + - - - - - - - - + - - - - - - - - +\n");
	refresh();
}

int count_empty() {
	unsigned int count = 0;
	for (unsigned int i=0; i<TabSize; i++) {
		for (int j = 0; j < TabSize; j++) {
			if (!board[i][j]) {
				count++;
			}
		}
	}
	return count;
}

void add_two(int empty) {
	int rand;
	if (empty>2) {
		rand = random() % (empty-1);
	}
	else {
		rand = empty;
	}

	for (unsigned int i=0; i<TabSize; i++) {
		for (int j=0; j<TabSize; j++) {
			if (!board[i][j]) {
				rand--;
			}
			if (!rand && !board[i][j]) {
				board[i][j]=2;
				break;
				break;
			}
		}
	}
}

int shift_board() {
	int to_return = 0;
	for (int i=0; i<TabSize; i++) {
		for (int j=TabSize-2; j>=0; j--){
			if (!board[i][j] && board[i][j+1]) {
				board[i][j] = board[i][j+1];
				board[i][j+1] = 0;
				to_return = 1;
			}
		}
	}
	return to_return;
}

int update_board() {
	shift_board();
	int HasMoved = 0;
	for (unsigned int i=0; i<TabSize; i++) {
		for (int j=0; j<TabSize-1; j++) {
			if (board[i][j]==board[i][j+1] && board[i][j]) {
				board[i][j] += board[i][j+1];
				board[i][j+1] = 0;
				score += board[i][j];
				HasMoved = 1;
			}
		}
	}
	shift_board();
	return HasMoved;
}

Key get_key() {
	int pressed = getch();
	if (pressed == up || pressed == down || pressed == left || pressed == right || pressed == backspace) {
		return pressed;
	}
}

void swap(int *a, int *b) {
	int c=*a;
	*a=*b;
	*b=c;
}

void mirror_board() {
	for (unsigned int i=0; i<TabSize; i++) {
		for (unsigned int j=0; j<TabSize/2; j++) {
			swap(&board[i][j], &board[i][(TabSize-1)-j]);
		}
	}
}

void pivot_board() {
	for (unsigned int i=0; i<TabSize; i++) {
        for (unsigned int j=i+1; j<TabSize; j++) {
            swap(&board[i][j], &board[j][i]);
        }
    }
}

int play(Key dir) {
	int to_return=0;
	int shift, update;
	switch(dir) {
		case up:
			pivot_board();
			shift = shift_board();
			update = update_board();
			if (shift || update) {
				to_return=1;
			}
			pivot_board();
			return to_return;

		case down:
			mirror_board();
			pivot_board();
			mirror_board();
			shift = shift_board();
			update = update_board();
			if (shift || update) {
				to_return=1;
			}
			mirror_board();
			pivot_board();
			mirror_board();
			return to_return;

		case left:
			shift = shift_board();
			update = update_board();
			if (shift || update) {
				to_return=1;
			}
			return to_return;

		case right:
			mirror_board();
			shift = shift_board();
			update = update_board();
			if (shift || update) {
				to_return=1;
			}
			mirror_board();
			return to_return;
		
		case backspace:
			DONE_2048();
	}
}

int game_over(int add) {
	int count_case = count_empty();
	if (!count_case) {
		printw("============= Game Over =============\n");
		printw("=========== (press a key) ===========");
		getch();
		return 1;
	}
	else if (count_case && add) {
		add_two(count_empty());
		display_board();
		return 0;
	}
	else {
		return 0;
	}
}

int main() {
	INIT_2048();
	init_board();
	int add = 1;
	while (!game_over(add)) {
		add = play(get_key());
	}
	DONE_2048();
}