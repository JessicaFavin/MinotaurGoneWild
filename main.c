#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "display.h"
#include "game.h"
#include "maze.h"

void print_bit(unsigned short s) {
  for(int i=15; i>=0; i--){
    int bit = (s>>i)&1;
    printf("%d",bit);
    if(i%4==0){
      printf(" ");
    }
  }
  printf("\n");
  return;
}

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
       //printf("wut? line:%d col:%d\n", maze->line, maze->col);
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
    do{
      maze = gener_random_maze();
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
  //------------------------------SMART MODE---------------------------------
  if(choice==3){
    //play_mode(maze);
    goto quit;
  }

  //------------------------------QUIT GAME---------------------------------
  quit:
  displaycursor();
  printf("\033[%d;1H",((2*maze->line)+4));
  if(!from_file){
    printf("Save file ? [y|n]\n");
    char c;
    scanf("\n %c", &c);
    if(c=='Y' || c=='y'){
      char location[256];
      printf("Where to ? ");
      scanf("%s",location);
      file = fopen(location, "wb");
      save_maze(maze, file);
      fclose(file);
    }
  }
  free_maze(maze);

  return 0;
}
