#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "maze.h"

#define DEFAULT_LINE 10
#define DEFAULT_COL 10

unsigned short ** init_maze_array(int line, int col) {
  unsigned short ** maze_array = calloc(line, sizeof(unsigned short*));
  for(int i=0; i<line; i++) {
    maze_array[i] = calloc(col, sizeof(unsigned short));
  }
  return maze_array;
}

void free_maze_array(unsigned short** maze_array, int line) {
  for(int i=0; i<line; i++) {
    free(maze_array[i]);
  }
  free(maze_array);
}

void free_maze(MAZE* maze) {
  free_maze_array(maze->maze_array, maze->line);
  free(maze);
}

MAZE* init_maze(int line, int col) {
  MAZE* maze = malloc(sizeof(MAZE));
  maze->maze_array = init_maze_array(line, col);
  /*
  maze->in = malloc(sizeof(POS));
  maze->out = malloc(sizeof(POS));
  maze->minotaur = malloc(sizeof(POS));
  */
  return maze;
}

MAZE* gener_maze_from_file(FILE* file) {
  FILE *fp;
  //generate maze
  fp = fopen("maze.txt", "r");
  int line, col, inX, inY, outX, outY, value;
  value = fscanf(fp, "%d %d %d %d %d %d\n", &line, &col, &inX, &inY, &outX, &outY);
  MAZE* maze = init_maze(line, col);
  maze->line = line;
  maze->col = col;
  maze->in.x = inX;
  maze->in.y = inY;
  maze->minotaur.x = inX;
  maze->minotaur.y = inY;
  maze->out.x= outX;
  maze->out.y= outY;
  if(value != 6){
    printf("File unreadable.\n");
    exit(2);
  }
  for(int i=0; i<line; i++) {
    for(int j=0; j<col; j++) {
      //check if value makes sense too
      value = fscanf(fp, "%hd", &maze->maze_array[i][j]);
      //printf("%d %d %hd\n", i, j, maze->maze_array[i][j]);
      if(value != 1){
        printf("File unreadable.\n");
        exit(2);
      }
    }
  }
  fclose(fp);
  return maze;
}

MAZE* gener_random_maze() {
  MAZE* maze = init_maze(DEFAULT_LINE, DEFAULT_COL);
  //generate random maze
  maze->in.x = 0;
  maze->in.y = 0;
  maze->minotaur.x = 0;
  maze->minotaur.y = 0;
  maze->out.x= DEFAULT_LINE;
  maze->out.y= DEFAULT_COL;
  return maze;
}
