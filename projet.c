// ldd: -lncurses
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>

typedef enum {
	up = KEY_UP,
	down = KEY_DOWN,
	left = KEY_LEFT,
	right = KEY_RIGHT,
	backspace = 127, // BACKSPACE NE FONCTIONNE PAS SUR MAC POUR MOI
} Key;

#define TabSize 4
int board[TabSize][TabSize];
unsigned int score = 0;

void INIT_2048() {
	initscr();
	start_color();
	raw();
	keypad(stdscr, TRUE);
	noecho();
	srand(time(NULL));

	init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);
    init_pair(6, COLOR_YELLOW, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);

}

void DONE_2048() {
	endwin();
	exit(0);
}

int set_color(int value) {
	int count=0;
	while (value>1) {
		value = value/2;
		count++;
	}
	return (count%7)+1;
    
}

void init_board() {
	for (unsigned int i=0; i<TabSize; i++) {
		for (unsigned int j=0; j<TabSize; j++) {
			board[i][j] = 0;
		}
	}
}

void display(char* val) {
    for (int i=0; i<TabSize; i++) {
        printw(val);
    }
}

void display_board() {
	move(0, 0);
	int color;
	printw("== 2048 =============================\n");
	printw("======================= score :%5i\n", score);
	for (int i=0; i<TabSize; i++) {
		display("+ - - - - - - - - ");
        printw("+\n|");
		display(" %17 |");
        printw("\n");
		printw("|");
		for (int j=0; j<TabSize; j++) {
            if (board[i][j]) {
				color = set_color(board[i][j]);
				attron(COLOR_PAIR(color));
				printw("%9i", board[i][j]);
				attroff(A_COLOR);
				printw("%9|");
			}
			else {
				printw("%18|");
			}
		}
        printw("\n|");
		display(" %17 |");
        printw("\n");
        
	}
    display("+ - - - - - - - - ");
    printw("+\n");
	refresh();
}

int count_empty() {
	unsigned int count = 0;
	for (unsigned int i=0; i<TabSize; i++) {
		for (int j=0; j<TabSize; j++) {
			if (!board[i][j]) {
				count++;
			}
		}
	}
	return count;
}

void add_two(int empty) {
	int n = rand() % empty;

	for (unsigned int i=0; i<TabSize; i++) {
		for (int j=0; j<TabSize; j++) {
			if (!board[i][j]) {
				if (n) {
					n--;
				}
				else {
					board[i][j] = 2;
					return;
				}
			}
		}
	}
}

int shift_board() {
    int to_return = 0;

    for (int i=0; i < TabSize; i++) {
        int empty = -1;
        for (int j=0; j < TabSize; j++) {
            if (board[i][j]==0) {
                if (empty==-1) {
                    empty=j;
                }
            } else {
                if (empty!=-1) {
                    board[i][empty]=board[i][j];
                    board[i][j] = 0;
                    to_return = 1;
                    empty++;
                }
            }
        }
    }

    return to_return;
}

int update_board() {
	int to_return = 0;
	if (shift_board()) {to_return=1;}
	for (unsigned int i=0; i<TabSize; i++) {
		for (int j=0; j<TabSize-1; j++) {
			if (board[i][j]==board[i][j+1] && board[i][j]) {
				board[i][j] += board[i][j+1];
				board[i][j+1] = 0;
				score += board[i][j];
				to_return = 1;
			}
		}
	}
	shift_board();
	return to_return;
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
			if (update_board()) {
				to_return=1;
			}
			pivot_board();
			return to_return;

		case down:
			mirror_board();
			pivot_board();
			mirror_board();
			if (update_board()) {
				to_return=1;
			}
			mirror_board();
			pivot_board();
			mirror_board();
			return to_return;

		case left:
			if (update_board()) {
				to_return=1;
			}
			return to_return;

		case right:
			mirror_board();
			if (update_board()) {
				to_return=1;
			}
			mirror_board();
			return to_return;
		
		case backspace:
			printw("=========== (press a key) ===========");
			getch();
			DONE_2048();
			return 0;
	}
}

int game_over(int add) {
	int count_case = count_empty();
	if (!count_case) {
		printw("============= Game Over =============\n");
		printw("=========== (press a key) ===========");
		refresh();
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