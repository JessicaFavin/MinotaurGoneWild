#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "display.h"
#include "game.h"
#include "maze.h"

int main(int argc, char* argv[]){
  int from_file = 0;
  char* filename;
  MAZE* maze;
  int choice = 0;
  FILE * file;
  hidecursor();
  system("clear");
  printf("\033[1;1H");
  //------------------------------FILE MAZE---------------------------------
  if(argc>1){
    filename = argv[1];
    file = fopen(filename, "r");
    if (file){
      from_file = 1;
       maze = gener_maze_from_file(file);
       fclose(file);
       int integrity = check_integrity(maze);
       if(!integrity) {
         printf("The maze is all wrong ! Do it again !\n");
         goto quit;
       }
    }else{
       printf("This file doesn't exists.\n");
       exit(1);
    }
  }
  //------------------------------RANDOM MAZE---------------------------------
  if(argc==1){
    int lines = 10, columns = 10;
    do{
      printf("How many lines ? ");
      scanf("%d",&lines);
    } while (lines<=0);
    do{
      printf("How many columns ? ");
      scanf("%d",&columns);
    } while (columns<=0);
    system("clear");

    do{
      maze = gener_random_maze(lines, columns);
    } while(!check_integrity(maze));
  }

  reinit(maze);

  choice = launch_menu();
  //------------------------------BERSERK MODE---------------------------------
  if(choice==1){
    berserk_mode(maze);
  }
  //------------------------------SMART MODE---------------------------------
  if(choice==2){
    smart_mode(maze);
  }
  //------------------------------PLAYER MODE---------------------------------
  if(choice==3){
    play_mode(maze);
  }

  //------------------------------QUIT GAME---------------------------------
  quit:
  displaycursor();
  if(choice!=4) {
    printf("\033[%d;1H",((2*maze->line)+4));
    if(!from_file){
      printf("Save file ? [y|n]\n");
      char c;
      scanf("\n %c", &c);
      if(c=='Y' || c=='y'){
        reinit(maze);
        char location[256];
        printf("Where to ? ");
        scanf("%s",location);
        file = fopen(location, "wb");
        save_maze(maze, file);
        fclose(file);
      }
    }
  }
  free_maze(maze);

  return 0;
}
