#ifndef MAZE_H
#define MAZE_H

#include <stdio.h>

typedef struct pos POS;
struct pos{
    int x;
    int y;
};

/*
le tableau 2D d’entiers
le nombre de lignes et de colonnes du labyrinthe,
la position en x et y de l’entrée/sortie du labyrinthe,
ainsi que la position en x et y du chercheur de chemin.
*/

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

#endif
