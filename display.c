#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <ncurses.h>

#define WIDTH 20
#define HEIGHT 7

int startx = 0;
int starty = 0;

char *choices[] = {
	"Berserk minautor",
	"Smart minotaur",
	"Quit"
};

int n_choices = sizeof(choices) / sizeof(char *);

void print_menu(WINDOW *menu_win, int highlight){
	int x, y, i;

	x = 2;
	y = 2;
	box(menu_win, 0, 0);
	for(i = 0; i < n_choices; ++i)
	{	if(highlight == i + 1) /* High light the present choice */
		{	wattron(menu_win, A_REVERSE);
			mvwprintw(menu_win, y, x, "%s", choices[i]);
			wattroff(menu_win, A_REVERSE);
		}
		else
		mvwprintw(menu_win, y, x, "%s", choices[i]);
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
	cbreak();	/* Line buffering disabled. pass on everything */
	int row,col;
	getmaxyx(stdscr,row,col);
	//width of game = 76
	startx = (col/3) - (WIDTH/2);
	starty = (row/3) - (HEIGHT/2);

	menu_win = newwin(HEIGHT, WIDTH, starty, startx);
	keypad(menu_win, TRUE);
  char * gameMenu = malloc(2048*sizeof(char));
  strcat(gameMenu, "            __  __ _             _                        \n");
  strcat(gameMenu, "           |  \\/  (_)_ __   ___ | |_ __ _ _  _ _ __ ___  \n");
  strcat(gameMenu, "           | |\\/| | | '_ \\ / _ \\| __/ _\\`| | | | '__/ _ \\ \n");
  strcat(gameMenu, "           | |  | | | | | | (_) | || (_| | |_| | | |  __/ \n");
  strcat(gameMenu, "           |_|__|_|_|_| |_|\\___/ \\__\\__,_|\\__,_|_|_ \\___| \n");
  strcat(gameMenu, "            / ___| ___  _ __   ___  \\ \\      / (_) | __| |\n");
  strcat(gameMenu, "           | |  _ / _ \\| '_ \\ / _ \\  \\ \\ /\\ / /| | |/ _` |\n");
  strcat(gameMenu, "           | |_| | (_) | | | |  __/   \\ V  V / | | | (_| |\n");
  strcat(gameMenu, "            \\____|\\___/|_| |_|\\___|    \\_/\\_/  |_|_|\\__,_|\n");
  strcat(gameMenu, "                                                          \n");
	mvprintw(3,0,gameMenu);
	refresh();
	print_menu(menu_win, highlight);
	while(1)
	{	c = wgetch(menu_win);
		switch(c)
		{	case KEY_UP:
			if(highlight == 1)
			highlight = n_choices;
			else
			--highlight;
			break;
			case KEY_DOWN:
			if(highlight == n_choices)
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
		if(choice != 0)	/* User did a choice come out of the infinite loop */
		break;
	}
	//mvprintw(23, 0, "You chose choice %d with choice string %s\n", choice, choices[choice - 1]);
	clrtoeol();
	refresh();
	endwin();
	return choice;
}
