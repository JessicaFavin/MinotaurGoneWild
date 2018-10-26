#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <ncurses.h>

#define MENUBOX_WIDTH 20
#define MENUBOX_HEIGHT 7

int startx = 0;
int starty = 0;

char *modes[] = {
	"Berserk minautor",
	"Smart minotaur",
	"Quit"
};

int mode_choices = sizeof(modes) / sizeof(char *);


char *manual[] = {
	"Random maze",
	"Maze from file",
	"Quit"
};
int manual_choices = sizeof(manual) / sizeof(char *);


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
  char * gameMenu = malloc(2048*sizeof(char));
  strcat(gameMenu, " __  __ _             _                        \n");
  strcat(gameMenu, "|  \\/  (_)_ __   ___ | |_ __ _ _  _ _ __ ___  \n");
  strcat(gameMenu, "| |\\/| | | '_ \\ / _ \\| __/ _\\`| | | | '__/ _ \\ \n");
  strcat(gameMenu, "| |  | | | | | | (_) | || (_| | |_| | | |  __/ \n");
  strcat(gameMenu, "|_|__|_|_|_| |_|\\___/ \\__\\__,_|\\__,_|_|_ \\___| \n");
  strcat(gameMenu, " / ___| ___  _ __   ___  \\ \\      / (_) | __| |\n");
  strcat(gameMenu, "| |  _ / _ \\| '_ \\ / _ \\  \\ \\ /\\ / /| | |/ _` |\n");
  strcat(gameMenu, "| |_| | (_) | | | |  __/   \\ V  V / | | | (_| |\n");
  strcat(gameMenu, " \\____|\\___/|_| |_|\\___|    \\_/\\_/  |_|_|\\__,_|\n");
  /*
  getmaxyx(stdscr,row,col);		// get the number of rows and columns
  mvprintw(row/2,(col-strlen(mesg))/2,"%s",mesg);
  */
	mvprintw(3,0,gameMenu);
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

void print_manual_menu(WINDOW *menu_win, int highlight){
	int x, y, i;

	x = 2;
	y = 2;
	box(menu_win, 0, 0);
	for(i = 0; i < manual_choices; ++i)
	{	if(highlight == i + 1) /* High light the present choice */
		{	wattron(menu_win, A_REVERSE);
			mvwprintw(menu_win, y, x, "%s", manual[i]);
			wattroff(menu_win, A_REVERSE);
		}
		else
		mvwprintw(menu_win, y, x, "%s", manual[i]);
		++y;
	}
	wrefresh(menu_win);
}

int launch_manual_menu() {
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
	startx = (col/3) - (MENUBOX_WIDTH/2);
	starty = (row/3) - (MENUBOX_HEIGHT/2);

	menu_win = newwin(MENUBOX_HEIGHT, MENUBOX_WIDTH, starty, startx);
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
	print_manual_menu(menu_win, highlight);
	while(1)
	{	c = wgetch(menu_win);
		switch(c)
		{	case KEY_UP:
			if(highlight == 1)
			highlight = manual_choices;
			else
			--highlight;
			break;
			case KEY_DOWN:
			if(highlight == manual_choices)
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
		print_manual_menu(menu_win, highlight);
		if(choice != 0)	/* User made a choice come out of the infinite loop */
		break;
	}
	clrtoeol();
	refresh();
	endwin();
	return choice;
}
