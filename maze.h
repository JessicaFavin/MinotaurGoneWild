#ifndef MAZE_H
#define MAZE_H

#include <stdio.h>
#define NORTH 0b1000
#define EAST  0b0100
#define SOUTH 0b0010
#define WEST  0b0001

typedef struct pos POS;
struct pos{
    int x;
    int y;
};

typedef struct maze MAZE;
struct maze{
    unsigned short** maze_array;
    int line;
    int col;
    POS in;
    POS out;
    POS minotaur;
};

unsigned short ** init_maze_array();
MAZE* init_maze();
int check_integrity(MAZE* maze);
MAZE* gener_maze_from_file(FILE* file);
MAZE* gener_random_maze();
void free_maze(MAZE* maze);
void reinit(MAZE* maze);
void save_maze(MAZE* maze, FILE* file);

#endif
