#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "display.h"
#include "game.h"
#include "maze.h"

int main(int argc, char* argv[]){
  int choice = 0;
  hidecursor();
  system("clear");
  printf("\033[1;1H");
  char* filename;
  MAZE* maze;
  //------------------------------FILE MAZE---------------------------------
  if(argc>1){
    filename = argv[1];
    FILE * file;
    file = fopen(filename, "r");
    if (file){
       maze = gener_maze_from_file(file);
       //printf("wut? line:%d col:%d\n", maze->line, maze->col);
       fclose(file);
    }else{
       printf("This file doesn't exists.\n");
       exit(1);
    }
  }
  //------------------------------RANDOM MAZE---------------------------------
  if(argc==1){
    maze = gener_random_maze();
  }
  int integrity = check_integrity(maze);
  if(!integrity) {
    printf("The maze is all wrong ! Do it again !\n");
    goto quit;
  }
  short_display_maze(maze);
  printf("\n");
  /*
  choice = launch_menu();
  //------------------------------BERSERK MODE---------------------------------
  if(choice==1){
    berserk_mode(maze);
  }
  //------------------------------SMART MODE---------------------------------
  if(choice==2){
    smart_mode(maze);
  }
  //------------------------------SMART MODE---------------------------------
  if(choice==3){
    play_mode(maze);
  }
  */
  //------------------------------QUIT GAME---------------------------------
  quit:
  free_maze(maze);
  displaycursor();
  printf("\033[%d;1H",((2*maze->line)+4));
  return 0;
}
