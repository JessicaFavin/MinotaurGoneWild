#ifndef GAME_H
#define GAME_H

#include "maze.h"

MAZE* gener_maze_from_file(FILE* file);
MAZE* gener_random_maze();
void berserk_mode(MAZE* maze);
void smart_mode(MAZE* maze);

#endif
