#include <ncurses.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char *simple_string = "Hello, world!";

    printf("Befor: COLS=%x; LINES=%x; stdscr=%p\n", COLS, LINES, stdscr);
    if( NULL == initscr()) {
	return __LINE__;
    }
    printf("AFTER: COLS=%x; LINES=%x; stdscr=%p\n", COLS, LINES, stdscr);

    curs_set(0);
    for ( int i = 0 ; i < strlen(simple_string) ; i++ ) {
	mvaddch(0, i * 2, simple_string[i]);
	mvaddch(i, 0 , simple_string[i]);
	refresh();
	sleep(1);
    }
    getch();
    endwin();
    return 0;
}
