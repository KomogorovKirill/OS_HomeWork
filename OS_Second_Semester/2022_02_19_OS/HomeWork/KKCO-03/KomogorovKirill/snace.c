#include <ncurses.h>

void up(int *x1, int *y1, int *x2, int *y2, int *x3, int *y3, int *x4, int *y4, int *x5, int *y5, const int lines)
{
	*x5 = *x4; *y5 = *y4;
	*x4 = *x3; *y4 = *y3;
	*x3 = *x2; *y3 = *y2;
	*x2 = *x1; *y2 = *y1;
	*y1 = (*y1 + lines - 1) % lines;
}

void right(int *x1, int *y1, int *x2, int *y2, int *x3, int *y3, int *x4, int *y4, int *x5, int *y5, const int cols)
{
	*x5 = *x4; *y5 = *y4;
	*x4 = *x3; *y4 = *y3;
	*x3 = *x2; *y3 = *y2;
	*x2 = *x1; *y2 = *y1;
	*x1 = (*x1 + 1) % cols;
}

void down(int *x1, int *y1, int *x2, int *y2, int *x3, int *y3, int *x4, int *y4, int *x5, int *y5, const int lines)
{
	*x5 = *x4; *y5 = *y4;
	*x4 = *x3; *y4 = *y3;
	*x3 = *x2; *y3 = *y2;
	*x2 = *x1; *y2 = *y1;
	*y1 = (*y1 + 1) % lines;
}

void left(int *x1, int *y1, int *x2, int *y2, int *x3, int *y3, int *x4, int *y4, int *x5, int *y5, const int cols)
{
	*x5 = *x4; *y5 = *y4;
	*x4 = *x3; *y4 = *y3;
	*x3 = *x2; *y3 = *y2;
	*x2 = *x1; *y2 = *y1;
	*x1 = (*x1 + cols - 1) % cols;
}


int main()
{
	initscr();
	
	int x1_1 = COLS / 2, y1_1 = LINES / 2 - 1, x2_1 = COLS / 2, y2_1 = LINES / 2 - 1, x3_1 = COLS / 2, y3_1 = LINES / 2 - 1, x4_1 = COLS / 2, y4_1 = LINES / 2 - 1, x5_1 = COLS / 2, y5_1 = LINES / 2 - 1;
	int x1_2 = COLS / 2 + 1, y1_2 = LINES / 2, x2_2 = COLS / 2 + 1, y2_2 = LINES / 2, x3_2 = COLS / 2 + 1, y3_2 = LINES / 2, x4_2 = COLS / 2 + 1, y4_2 = LINES / 2, x5_2 = COLS / 2 + 1, y5_2 = LINES / 2;	
	int x1_3 = COLS / 2, y1_3 = LINES / 2 + 1, x2_3 = COLS / 2, y2_3 = LINES / 2 + 1, x3_3 = COLS / 2, y3_3 = LINES / 2 + 1, x4_3 = COLS / 2, y4_3 = LINES / 2 + 1, x5_3 = COLS / 2, y5_3 = LINES / 2 + 1;
	int x1_4 = COLS / 2 - 1, y1_4 = LINES / 2, x2_4 = COLS / 2 - 1, y2_4 = LINES / 2, x3_4 = COLS / 2 - 1, y3_4 = LINES / 2, x4_4 = COLS / 2 - 1, y4_4 = LINES / 2, x5_4 = COLS / 2 - 1, y5_4 = LINES / 2;

	int ch = 0;
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	while (ch != 'q')
	{

		mvaddch(y1_1, x1_1, '*');
		mvaddch(y2_1, x2_1, '*');
		mvaddch(y3_1, x3_1, '*');
		mvaddch(y4_1, x4_1, '*');
		mvaddch(y5_1, x5_1, '*');

		mvaddch(y1_2, x1_2, '*');
		mvaddch(y2_2, x2_2, '*');
		mvaddch(y3_2, x3_2, '*');
		mvaddch(y4_2, x4_2, '*');
		mvaddch(y5_2, x5_2, '*');

		mvaddch(y1_3, x1_3, '*');
		mvaddch(y2_3, x2_3, '*');
		mvaddch(y3_3, x3_3, '*');
		mvaddch(y4_3, x4_3, '*');
		mvaddch(y5_3, x5_3, '*');
		
		mvaddch(y1_4, x1_4, '*');
		mvaddch(y2_4, x2_4, '*');
		mvaddch(y3_4, x3_4, '*');
		mvaddch(y4_4, x4_4, '*');
		mvaddch(y5_4, x5_4, '*');

		ch = getch();

		mvaddch(y1_1, x1_1, ' ');
		mvaddch(y2_1, x2_1, ' ');
		mvaddch(y3_1, x3_1, ' ');
		mvaddch(y4_1, x4_1, ' ');
		mvaddch(y5_1, x5_1, ' ');

		mvaddch(y1_2, x1_2, ' ');
		mvaddch(y2_2, x2_2, ' ');
		mvaddch(y3_2, x3_2, ' ');
		mvaddch(y4_2, x4_2, ' ');
		mvaddch(y5_2, x5_2, ' ');

		mvaddch(y1_3, x1_3, ' ');
		mvaddch(y2_3, x2_3, ' ');
		mvaddch(y3_3, x3_3, ' ');
		mvaddch(y4_3, x4_3, ' ');
		mvaddch(y5_3, x5_3, ' ');

		mvaddch(y1_4, x1_4, ' ');
		mvaddch(y2_4, x2_4, ' ');
		mvaddch(y3_4, x3_4, ' ');
		mvaddch(y4_4, x4_4, ' ');
		mvaddch(y5_4, x5_4, ' ');

		switch(ch)
		{
			case KEY_UP:
				up(&x1_1, &y1_1, &x2_1, &y2_1, &x3_1, &y3_1, &x4_1, &y4_1, &x5_1, &y5_1, LINES);
				right(&x1_2, &y1_2, &x2_2, &y2_2, &x3_2, &y3_2, &x4_2, &y4_2, &x5_2, &y5_2, COLS);
				down(&x1_3, &y1_3, &x2_3, &y2_3, &x3_3, &y3_3, &x4_3, &y4_3, &x5_3, &y5_3, LINES);
				left(&x1_4, &y1_4, &x2_4, &y2_4, &x3_4, &y3_4, &x4_4, &y4_4, &x5_4, &y5_4, COLS);
				break;
			case KEY_RIGHT:
				right(&x1_1, &y1_1, &x2_1, &y2_1, &x3_1, &y3_1, &x4_1, &y4_1, &x5_1, &y5_1, COLS);
				down(&x1_2, &y1_2, &x2_2, &y2_2, &x3_2, &y3_2, &x4_2, &y4_2, &x5_2, &y5_2, LINES);
				left(&x1_3, &y1_3, &x2_3, &y2_3, &x3_3, &y3_3, &x4_3, &y4_3, &x5_3, &y5_3, COLS);
				up(&x1_4, &y1_4, &x2_4, &y2_4, &x3_4, &y3_4, &x4_4, &y4_4, &x5_4, &y5_4, LINES);
				break;
			case KEY_DOWN:
				down(&x1_1, &y1_1, &x2_1, &y2_1, &x3_1, &y3_1, &x4_1, &y4_1, &x5_1, &y5_1, LINES);
				left(&x1_2, &y1_2, &x2_2, &y2_2, &x3_2, &y3_2, &x4_2, &y4_2, &x5_2, &y5_2, COLS);
				up(&x1_3, &y1_3, &x2_3, &y2_3, &x3_3, &y3_3, &x4_3, &y4_3, &x5_3, &y5_3, LINES);
				right(&x1_4, &y1_4, &x2_4, &y2_4, &x3_4, &y3_4, &x4_4, &y4_4, &x5_4, &y5_4, COLS);
				break;
			case KEY_LEFT:
				left(&x1_1, &y1_1, &x2_1, &y2_1, &x3_1, &y3_1, &x4_1, &y4_1, &x5_1, &y5_1, COLS);
				up(&x1_2, &y1_2, &x2_2, &y2_2, &x3_2, &y3_2, &x4_2, &y4_2, &x5_2, &y5_2, LINES);
				right(&x1_3, &y1_3, &x2_3, &y2_3, &x3_3, &y3_3, &x4_3, &y4_3, &x5_3, &y5_3, COLS);
				down(&x1_4, &y1_4, &x2_4, &y2_4, &x3_4, &y3_4, &x4_4, &y4_4, &x5_4, &y5_4, LINES);
				break;			
			default:
				break;
		}
	}
	endwin();
	return 0;
}
