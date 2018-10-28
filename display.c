#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <ncurses.h>
#include "maze.h"

#define MENUBOX_WIDTH 20
#define MENUBOX_HEIGHT 7

int startx = 0;
int starty = 0;

char *modes[] = {
	"Berserk minotaur",
	"Smart minotaur",
	"Be the minotaur",
	"Quit"
};

int mode_choices = sizeof(modes) / sizeof(char *);



void hidecursor() {
  system("setterm -cursor off");
}

void displaycursor() {
  system("setterm -cursor on");
}

void print_menu(WINDOW *menu_win, int highlight){
	int x, y, i;

	x = 2;
	y = 2;
	box(menu_win, 0, 0);
	for(i = 0; i < mode_choices; ++i)
	{	if(highlight == i + 1) // High light the present choice
		{	wattron(menu_win, A_REVERSE);
			mvwprintw(menu_win, y, x, "%s", modes[i]);
			wattroff(menu_win, A_REVERSE);
		}
		else
		mvwprintw(menu_win, y, x, "%s", modes[i]);
		++y;
	}
	wrefresh(menu_win);
}

int launch_menu() {
	WINDOW *menu_win;
	int highlight = 1;
	int choice = 0;
	int c;
	initscr();
	clear();
	noecho();
	cbreak();	// Line buffering disabled. pass on everything
	int row,col;
	getmaxyx(stdscr,row,col);
	//width of game = 76
	startx = (col/3) - (MENUBOX_WIDTH/2);
	starty = (row/3) - (MENUBOX_HEIGHT/2);

	menu_win = newwin(MENUBOX_HEIGHT, MENUBOX_WIDTH, starty, startx);
	keypad(menu_win, TRUE);
  mvprintw(3, (col/3)-(47/2), " __  __ _             _                        ");
  mvprintw(4, (col/3)-(47/2), "|  \\/  (_)_ __   ___ | |_ __ _ _  _ _ __ ___  ");
  mvprintw(5, (col/3)-(47/2), "| |\\/| | | '_ \\ / _ \\| __/ _\\`| | | | '__/ _ \\ ");
  mvprintw(6, (col/3)-(47/2), "| |  | | | | | | (_) | || (_| | |_| | | |  __/ ");
  mvprintw(7, (col/3)-(47/2), "|_|__|_|_|_| |_|\\___/ \\__\\__,_|\\__,_|_|_ \\___| ");
  mvprintw(8, (col/3)-(47/2), " / ___| ___  _ __   ___  \\ \\      / (_) | __| |");
  mvprintw(9, (col/3)-(47/2), "| |  _ / _ \\| '_ \\ / _ \\  \\ \\ /\\ / /| | |/ _` |");
  mvprintw(10, (col/3)-(47/2), "| |_| | (_) | | | |  __/   \\ V  V / | | | (_| |");
  mvprintw(11, (col/3)-(47/2), " \\____|\\___/|_| |_|\\___|    \\_/\\_/  |_|_|\\__,_|");
  /*
  getmaxyx(stdscr,row,col);		// get the number of rows and columns
  mvprintw(row/2,(col-strlen(mesg))/2,"%s",mesg);
  */
	refresh();
	print_menu(menu_win, highlight);
	while(1)
	{	c = wgetch(menu_win);
		switch(c)
		{	case KEY_UP:
			if(highlight == 1)
			highlight = mode_choices;
			else
			--highlight;
			break;
			case KEY_DOWN:
			if(highlight == mode_choices)
			highlight = 1;
			else
			++highlight;
			break;
			case 10:
			choice = highlight;
			break;
			default:
			break;
		}
		print_menu(menu_win, highlight);
		if(choice != 0)	// User made a choice come out of the infinite loop
		break;
	}
	clrtoeol();
	refresh();
	endwin();
	return choice;
}
/*
+--+--+--+--+--+
|  |  |  |  |  |
+--+--+--+--+--+
|  |  |  |  |  |
+--+--+--+--+--+
|  |  |  |  |  |
+--+--+--+--+--+
(line+1), (line+2)
(col+1)
0000 0000 0000 0000
W 				   V NESO
in the way   visited orientation

for every cell consider 2 lines

0,0 : 1,1 and 2,1

1,3 : 3,7 and 4,7
(3*line)+1 (3*line)+2
(2*col)+1
*/

void erase_minotaur(MAZE* maze) {
	POS minotaur = maze->minotaur;
	printf("\033[%d;%dH🌱", (minotaur.x*2)+2, (minotaur.y*3)+2);
	fflush(stdout);
}

void display_minotaur(MAZE* maze) {
	POS minotaur = maze->minotaur;
	printf("\033[%d;%dH🐮", (minotaur.x*2)+2, (minotaur.y*3)+2);
	fflush(stdout);
}

void display_out(MAZE* maze) {
	POS out = maze->out;
	printf("\033[%d;%dH🥩", (out.x*2)+2, (out.y*3)+2);
	fflush(stdout);
}

void display_maze(MAZE* maze) {
	int line = maze->line;
	int col = maze->col;
	for(int i=0; i<line; i++) {
    for(int j=0; j<col; j++) {
			int north = (maze->maze_array[i][j]>>3)&0x1;
			int east 	= (maze->maze_array[i][j]>>2)&0x1;
			int south = (maze->maze_array[i][j]>>1)&0x1;
			int west 	= (maze->maze_array[i][j])&0x1;
			//printf("N:%d E:%d S:%d W:%d\n",north, east, south, west);
			if(north){
				printf("\033[%d;%dH+--",((2*i)+1),((3*j)+1));
			} else {
				printf("\033[%d;%dH+  ",((2*i)+1),((3*j)+1));
			}
			//end of line
			if (j==maze->col-1) {
				printf("+");
			}

			if(west){
				printf("\033[%d;%dH|",((2*i)+2),((3*j)+1));
			} else {
				printf("\033[%d;%dH ",((2*i)+2),((3*j)+1));
			}
			printf("  ");
			if(east && j==maze->col-1){
				printf("|\n");
			}
			if(south && i==maze->line-1){
				printf("\033[%d;%dH+--",((2*i)+3),((3*j)+1));
				if (j==maze->col-1) {
					printf("+\n");
				}
			}
    }
  }
	fflush(stdout);
	display_minotaur(maze);
	display_out(maze);
}
