// ldd: -lncurses
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>

#define Taille 4

typedef enum
{
	up = KEY_UP,
	down = KEY_DOWN,
	left = KEY_LEFT,
	right = KEY_RIGHT,
	backspace = KEY_BACKSPACE,
} Key;

int board[Taille][Taille];
unsigned int score = 0;

void INIT_2048()
{
	initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();
	srand(time(NULL));
}

void DONE_2048()
{
	endwin();
	exit(0);
}

void init_board()
{
	for (unsigned int i = 0; i < Taille; i++)
	{
		for (unsigned int j = 0; j < Taille; j++)
		{
			board[i][j] = 0;
		}
	}
}

void display_board()
{
	move(0, 0);
	printw("== 2048 =============================\n");
	printw("======================= score :%5i\n", score);
	for (int i = 0; i < Taille; i++)
	{
		printw("+ - - - - - - - - + - - - - - - - - + - - - - - - - - + - - - - - - - - +\n");
		printw("| %17 | %17 | %17 | %17 |\n");
		printw("|");
		for (int j = 0; j < Taille; j++)
		{
			if (board[i][j])
			{
				printw("%9i%9 |", board[i][j]);
			}
			else
			{
				printw("%18|");
			}
		}
		printw("\n| %17 | %17 | %17 | %17 |\n");
	}
	printw("+ - - - - - - - - + - - - - - - - - + - - - - - - - - + - - - - - - - - +\n");
	refresh();
}

int count_empty()
{
	unsigned int count = 0;
	for (unsigned int i = 0; i < Taille; i++)
	{
		for (int j = 0; j < Taille; j++)
		{
			if (!board[i][j])
			{
				count++;
			}
		}
	}
	return count;
}

void add_two(int empty)
{
	int rand;
	if (empty > 2)
	{
		rand = random() % (empty - 1);
	}
	else
	{
		rand = empty;
	}

	for (unsigned int i = 0; i < Taille; i++)
	{
		for (int j = 0; j < Taille; j++)
		{
			if (!board[i][j])
			{
				rand--;
			}
			if (!rand && !board[i][j])
			{
				board[i][j] = 2;
				break;
				break;
			}
		}
	}
}

int shift_board()
{
	unsigned int to_return = 0;
	int i, j;
	for (i = 0; i < Taille; i++)
	{
		for (j = Taille - 2; j >= 0; j--)
		{
			if (!board[i][j])
			{
				board[i][j] = board[i][j + 1];
				board[i][j + 1] = 0;
				to_return = 1;
			}
		}
	}
	return to_return;
}

int update_board()
{
	shift_board();
	int i, j, to_return = 0;
	for (i = 0; i < Taille; i++)
	{
		for (j = 0; j < Taille - 1; j++)
		{
			if (board[i][j] == board[i][j + 1])
			{
				board[i][j] += board[i][j + 1];
				board[i][j + 1] = 0;
				score += board[i][j];
				to_return = 1;
			}
		}
	}
	shift_board();
	return to_return;
}

Key get_key()
{
	int pressed = getch();
	if (pressed == up || pressed == down || pressed == left || pressed == right || pressed == backspace)
	{
		return pressed;
	}
}

int game_over(int add)
{
	int count_case = count_empty();
	if (!count_case)
	{
		printw("============= Game Over =============\n");
		printw("=========== (press a key) ===========");
		getch();
		return 1;
	}
	else if (count_case && add)
	{
		add_two(count_empty());
		display_board();
		return 0;
	}
	else
	{
		return 0;
	}
}

int main()
{
	int add = 1;
	INIT_2048();
	init_board();
	while (!game_over(add))
	{
		get_key()
			update_board();
	}

	DONE_2048();
}