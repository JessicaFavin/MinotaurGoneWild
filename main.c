#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "display.h"
#include "game.h"

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
       fclose(file);
    }else{
       printf("This file doesn't exists.\n");
       exit(1);
    }
  }
  //------------------------------RANDOM MAZE---------------------------------
  if(argc==1){
    //maze = gener_random_maze();
  }
  //------------------------------QUIT GAME---------------------------------
  if(choice==3){
    goto quit;
  }
  choice = launch_menu();
  //------------------------------BERSERK MODE---------------------------------
  if(choice==1){
    berserk_mode(maze);
  }
  //------------------------------SMART MODE---------------------------------
  if(choice==2){
    smart_mode(maze);
  }
  //------------------------------QUIT GAME---------------------------------
  displaycursor();
  quit:
  return 0;
}
