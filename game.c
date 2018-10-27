#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "maze.h"
#include "display.h"

/*
char key_pressed() {
    struct termios oldterm, newterm;
    int oldfd;
    char c, result = 0;
    tcgetattr(STDIN_FILENO, &oldterm);
    newterm = oldterm;
    newterm.c_lflag &= ~ (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newterm);
    oldfd = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldfd | O_NONBLOCK);
    c = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldterm);
    fcntl(STDIN_FILENO, F_SETFL, oldfd);
    if(c!=EOF){
        ungetc(c, stdin);
        result = getchar();
    }
    return result;
}

int player_actions(char c, MAZE* maze){
    int b = 1;
    POS minotaur = maze->minotaur;
    if((c=='d'||c=='C') && minotaur.y<maze->col){
        move_minotaur(maze, 'W');
    }
    if((c=='q'||c=='D') && minotaur.y>0){
        move_minotaur(maze, 'E');
    }
    if((c=='A' || c=='z') && minotaur.x>0){
      move_minotaur(maze, 'N');

    }
    if((c=='B' || c=='s')  && minotaur.x<maze->line){
      move_minotaur(maze, 'S');
    }
    if(c=='l'){
        b = 0;
    }
    return b;
}
*/

void move_minotaur(MAZE* maze, int direction) {
  erase_minotaur(maze);
  POS minotaur = maze->minotaur;
  switch(direction) {
    case 'N':
      if (minotaur.x>0)
        maze->minotaur.x -= 1;
      break;
    case 'E':
      if (minotaur.y<maze->col)
        maze->minotaur.y += 1;
      break;
    case 'S':
      if (minotaur.x<maze->line)
        maze->minotaur.x += 1;
      break;
    case 'W':
      if (minotaur.y>0)
        maze->minotaur.y -= 1;
      break;
  }
  display_minotaur(maze);
  //sleep(1);
}

void berserk_mode(MAZE* maze) {
  int win = 0;
  POS out = maze->out;
  int northDoor, eastDoor, southDoor, westDoor, visited, rightWay;
  unsigned short *cell_ptr, cell, northCell, eastCell, southCell, westCell;
  display_maze(maze);
  while(!win){
    POS minotaur = maze->minotaur;
    cell_ptr = &maze->maze_array[minotaur.x][minotaur.y];
    cell = maze->maze_array[minotaur.x][minotaur.y];
    if (minotaur.x>0) {
      northCell = maze->maze_array[minotaur.x-1][minotaur.y];
    } else {
      northCell = 32;
    }
    if (minotaur.y<maze->col) {
      eastCell = maze->maze_array[minotaur.x][minotaur.y+1];
    } else {
      eastCell = 32;
    }
    if (minotaur.x<maze->line) {
      southCell = maze->maze_array[minotaur.x+1][minotaur.y];
    } else {
      southCell = 32;
    }
    if (minotaur.y>0) {
      westCell = maze->maze_array[minotaur.x][minotaur.y-1];
    } else {
      westCell = 32;
    }

    printf("\033[12;0H 15 ? %d", (cell>>15)&0x1);
    printf("\033[13;0H 4 ? %d", (cell>>4)&0x1);
    printf("\033[14;0H 3 ? %d", (cell>>3)&0x1);
    printf("\033[15;0H 2 ? %d", (cell>>2)&0x1);
    printf("\033[16;0H 1 ? %d", (cell>>1)&0x1);
    printf("\033[17;0H 0 ? %d", (cell>>0)&0x1);
    printf("\033[18;0H cell ? %d", cell);
    fflush(stdout);
    sleep(2);

    rightWay  = (cell>>15)&0x1;
    visited   = (cell>>4)&0x1;
    northDoor = (cell>>3)&0x1;
    eastDoor 	= (cell>>2)&0x1;
    southDoor = (cell>>1)&0x1;
    westDoor 	= (cell)&0x1;
    /*
    SET VISITED AND RIGHT WAY
    */
    if(!visited) {
      //Visited cell
      *cell_ptr += 16;
      //maze->maze_array[minotaur.x][minotaur.y] = cell | 0x10000;
      printf("\033[19;0Hmaze_array ? %d", *cell_ptr);
      fflush(stdout);
      sleep(1);
    }
    if(!rightWay){
      //Cell is on the right way
      *cell_ptr = cell | 0x1000000000000000;
      printf("\033[20;0Hright_way now ? %d", maze->maze_array[minotaur.x][minotaur.y]);
    }
    if(!northDoor && (northCell!=32 && !(northCell>>4)&0x1)) {
      move_minotaur(maze, 'N');
    } else if (!eastDoor && (eastCell!=32 && !(eastCell>>4)&0x1)) {
      move_minotaur(maze, 'E');
    } else if (!southDoor && (southCell!=32 && !(southCell>>4)&0x1)) {
      move_minotaur(maze, 'S');
    } else if (!westDoor && (westCell!=32 && !(westCell>>4)&0x1)) {
      move_minotaur(maze, 'W');
    } else {
      maze->maze_array[minotaur.x][minotaur.y] = cell & 0x0111111111111111;
      /*
      sleep(2);
      printf("\033[3;10H                                               ");
      printf("\033[3;10H    The minotaur is blocked. Rage quiting !    ");
      printf("\033[3;10H                                               ");
      exit(12);
      */
    }
    //minautor is on the out cell
    win = (minotaur.x==out.x)&&(minotaur.y==out.y);
  }
  return;
}

void smart_mode(MAZE* maze) {
  return;
}
