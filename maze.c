#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "maze.h"
#include "game.h"

#define DEFAULT_LINE 10
#define DEFAULT_COL 10

unsigned short ** init_maze_array(int line, int col) {
  unsigned short ** maze_array = calloc(line, sizeof(unsigned short*));
  for(int i=0; i<line; i++) {
    maze_array[i] = calloc(col, sizeof(unsigned short));
    for(int j=0; j<col; j++) {
      maze_array[i][j] = 0b1111;
    }
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

void reinit(MAZE* maze) {
  for(int i=0; i<maze->line; i++) {
    for(int j=0; j<maze->col; j++) {
      maze->maze_array[i][j] &= 0b1111;
    }
  }
  maze->minotaur.x = maze->in.x;
  maze->minotaur.y = maze->in.y;
}

int walls_make_sense(MAZE* maze) {
  for(int i=0; i<DEFAULT_LINE; i++) {
    for(int j=0; j<DEFAULT_COL; j++) {
      if(i!=0){
        if((maze->maze_array[i][j]&NORTH)==NORTH && (maze->maze_array[i-1][j]&SOUTH)!=SOUTH){
          printf("N%d %d %d %d\n",i, j, (maze->maze_array[i][j]&NORTH),(maze->maze_array[i-1][j]&SOUTH));
          return 0;
        }
      }
      if(j!=0){
        if((maze->maze_array[i][j]&WEST)==NORTH && (maze->maze_array[i][j-1]&EAST)!=EAST){
          printf("W%d %d %d %d\n",i, j, (maze->maze_array[i][j]&WEST),(maze->maze_array[i][j-1]&EAST));
          return 0;
        }
      }
      if(i!=DEFAULT_LINE-1){
        if((maze->maze_array[i][j]&SOUTH)==NORTH && (maze->maze_array[i+1][j]&NORTH)!=NORTH){
          printf("S%d %d %d %d\n",i, j, (maze->maze_array[i][j]&SOUTH),(maze->maze_array[i+1][j]&NORTH));
          return 0;
        }
      }
      if(j!=DEFAULT_COL-1){
        if((maze->maze_array[i][j]&EAST)==NORTH && (maze->maze_array[i][j+1]&WEST)!=WEST){
          printf("E%d %d %d %d\n",i, j, (maze->maze_array[i][j]&EAST),(maze->maze_array[i][j+1]&WEST));
          return 0;
        }
      }

    }
  }
  return 1;
}

int check_integrity(MAZE* maze) {
  //if maze is wrong
  if(walls_make_sense(maze)==1 && valid_labyrinth(maze)==1){
    return 1;
  }
  return 0;
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
  //printf("\033[35;0H");
  for(int i=0; i<DEFAULT_LINE; i++) {
    for(int j=0; j<DEFAULT_COL; j++) {
      //printf("%d:%d can destroy : ",i,j);
      if(i!=0){
        destroy = rand() % 3;
        if(destroy==0){
          //Destroy NORTH
          maze->maze_array[i][j] &=  ~NORTH;
          maze->maze_array[i-1][j] &= ~SOUTH;

        }
      }
      if(j!=0){
        destroy = rand() % 3;
        if(destroy==0){
          //Destroy WEST
          maze->maze_array[i][j] &=  ~WEST;
          maze->maze_array[i][j-1] &=  ~EAST;
        }
      }
      if(i!=DEFAULT_LINE-1){
        destroy = rand() % 3;
        if(destroy==0){
          //Destroy SOUTH
          maze->maze_array[i][j] &= ~SOUTH;
          maze->maze_array[i+1][j] &=  ~NORTH;

        }
      }
      if(j!=DEFAULT_COL-1){
        destroy = rand() % 3;
        if(destroy==0){
          //Destroy EAST
          maze->maze_array[i][j] &= ~EAST;
          maze->maze_array[i][j+1] &= ~WEST;
        }
      }

    }
  }
  return maze;
}

void save_maze(MAZE* maze, FILE* file) {
  int line = maze->line;
  int col = maze->col;
  fprintf(file, "%d %d %d %d %d %d\n", line, col, maze->in.x, maze->in.y, maze->out.x, maze->out.y);
  for(int i=0; i<line; i++) {
    for(int j=0; j<col; j++) {
      fprintf(file, "%hu ",maze->maze_array[i][j]);
    }
    fprintf(file, "\n");
  }
}
