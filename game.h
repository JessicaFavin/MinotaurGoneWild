#ifndef GAME_H
#define GAME_H

#include "maze.h"

#define NORTH 0b1000
#define EAST  0b0100
#define SOUTH 0b0010
#define WEST  0b0001

//char key_pressed();
void move_minotaur(MAZE* maze, int direction);
void berserk_mode(MAZE* maze);
void smart_mode(MAZE* maze);
//void play_mode(MAZE* maze);
int valid_labyrinth(MAZE* maze);

#endif
