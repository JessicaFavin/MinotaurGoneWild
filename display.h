#ifndef DISPLAY_H
#define DISPLAY_H

#include "maze.h"

int launch_menu();
void hidecursor();
void displaycursor();
void display_minotaur(MAZE* maze);
void erase_minotaur(MAZE* maze);
void display_out(MAZE* maze);
void display_maze(MAZE* maze);

#endif
