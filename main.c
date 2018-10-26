#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "display.h"

int main(int argc, char* argv[]){
  int choice = 0;
  hidecursor();
  system("clear");
  printf("\033[1;1H");
  choice = launch_manual_menu();
  //------------------------------RANDOM MAZE---------------------------------
  if(choice==1){
    //maze = random_maze();
  }
  //------------------------------FILE MAZE---------------------------------
  if(choice==2){
    //scanf for filename ?
    //maze = maze_from_file();
  }
  //------------------------------QUIT GAME---------------------------------
  if(choice==3){
    goto quit;
  }
  choice = launch_menu();
  //------------------------------BERSERK MODE---------------------------------
  if(choice==1){
    //berserk_mode(maze);
  }
  //------------------------------SMART MODE---------------------------------
  if(choice==2){
    //smart_mode(maze);
  }
  //------------------------------QUIT GAME---------------------------------
  displaycursor();
  quit:
  return 0;
}
