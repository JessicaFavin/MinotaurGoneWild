#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
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
  return maze;
}

int check_integrity(MAZE* maze) {
  //if maze is wrong
  //return 0,
  return 1;
}

MAZE* gener_maze_from_file(FILE* file) {
  //generate maze
  int line, col, inX, inY, outX, outY, value;
  value = fscanf(file, "%d %d %d %d %d %d\n", &line, &col, &inX, &inY, &outX, &outY);
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
      value = fscanf(file, "%hd", &maze->maze_array[i][j]);
      //printf("%d %d %hd\n", i, j, maze->maze_array[i][j]);
      if(value != 1){
        printf("File unreadable.\n");
        exit(2);
      }
    }
  }
  return maze;
}

MAZE* gener_random_maze() {
  srand(time(NULL));
  MAZE* maze = init_maze(DEFAULT_LINE, DEFAULT_COL);
  //generate random maze
  maze->line = DEFAULT_LINE;
  maze->col = DEFAULT_COL;
  maze->out.x= (rand()%DEFAULT_LINE);
  maze->out.y= (rand()%DEFAULT_LINE);
  maze->in.x = (rand()%DEFAULT_LINE);
  maze->in.y = (rand()%DEFAULT_COL);
  while(maze->in.x==maze->out.x && maze->in.y==maze->out.y) {
    maze->in.x = (rand()%DEFAULT_LINE);
    maze->in.y = (rand()%DEFAULT_COL);
  }
  maze->minotaur.x = maze->in.x;
  maze->minotaur.y = maze->in.y;

  int destroy;
  for(int i=0; i<DEFAULT_LINE; i++) {
    for(int j=0; j<DEFAULT_COL; j++) {
      unsigned short cell = 15;
      maze->maze_array[i][j] = cell;
      //printf("%d:%d can destroy : ",i,j);
      if(i!=0){
        // You can destroy NORTH
        //printf("NORTH ");
        destroy = rand() % 3;
        if(destroy!=0){
          maze->maze_array[i][j] = maze->maze_array[i][j] & 0b0111;
          maze->maze_array[i-1][j] = maze->maze_array[i-1][j] & 01101;
        }
      }
      if(j!=0){
        // You can destroy WEST
        //printf("WEST ");
        destroy = rand() % 3;
        if(destroy!=0){
          maze->maze_array[i][j] = maze->maze_array[i][j] & 0b1110;
          maze->maze_array[i][j-1] = maze->maze_array[i][j-1] & 0b1011;
        }
      }
      if(i!=DEFAULT_LINE-1){
        // You can destroy SOUTH
        //printf("SOUTH ");
        destroy = rand() % 3;
        if(destroy!=0){
          maze->maze_array[i][j] = maze->maze_array[i][j] & 0b1101;
          maze->maze_array[i+1][j] = maze->maze_array[i+1][j] & 0b0111;
        }
      }
      if(j!=DEFAULT_COL-1){
        // You can destroy EAST
        //printf("EAST ");
        destroy = rand() % 3;
        if(destroy!=0){
          maze->maze_array[i][j] = maze->maze_array[i][j] & 0b1011;
          maze->maze_array[i][j+1] = maze->maze_array[i][j+1] & 0b1110;
        }
      }
      printf("\n");
    }
  }
  return maze;
}
