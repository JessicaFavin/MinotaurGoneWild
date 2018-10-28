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
    if((c=='d'||c=='C') && minotaur.y<maze->col-1){
        move_minotaur(maze, 'W');
    }
    if((c=='q'||c=='D') && minotaur.y>0){
        move_minotaur(maze, 'E');
    }
    if((c=='A' || c=='z') && minotaur.x>0){
      move_minotaur(maze, 'N');

    }
    if((c=='B' || c=='s')  && minotaur.x<maze->line-1){
      move_minotaur(maze, 'S');
    }
    if(c=='l'){
        b = 0;
    }
    return b;
}
*/

void move_minotaur(MAZE* maze, int direction) {
  sleep(1);
  erase_minotaur(maze);
  POS minotaur = maze->minotaur;
  switch(direction) {
    case 'N':
      if (minotaur.x>0)
        maze->minotaur.x -= 1;
      break;
    case 'E':
      if (minotaur.y<maze->col-1)
        maze->minotaur.y += 1;
      break;
    case 'S':
      if (minotaur.x<maze->line-1)
        maze->minotaur.x += 1;
      break;
    case 'W':
      if (minotaur.y>0)
        maze->minotaur.y -= 1;
      break;
  }
  display_minotaur(maze);
  sleep(0);
}

int visitedCell(unsigned short cell) {
  return (cell>>4)&0b1;
}

int on_the_right_way(unsigned short cell) {
  return (cell>>15)&0b1;
}

void berserk_mode(MAZE* maze) {
  int win = 0;
  POS out = maze->out;
  int northDoor, eastDoor, southDoor, westDoor, visited, rightWay;
  unsigned short cell, northCell, eastCell, southCell, westCell;
  display_maze(maze);
  POS minotaur = maze->minotaur;
  while(win==0){
    cell = maze->maze_array[minotaur.x][minotaur.y];
    if (minotaur.x>0) {
      northCell = maze->maze_array[minotaur.x-1][minotaur.y];
    } else {
      northCell = 32;
    }
    if (minotaur.y<maze->col-1) {
      eastCell = maze->maze_array[minotaur.x][minotaur.y+1];
    } else {
      eastCell = 32;
    }
    if (minotaur.x<maze->line-1) {
      southCell = maze->maze_array[minotaur.x+1][minotaur.y];
    } else {
      southCell = 32;
    }
    if (minotaur.y>0) {
      westCell = maze->maze_array[minotaur.x][minotaur.y-1];
    } else {
      westCell = 32;
    }

    rightWay  = on_the_right_way(cell);
    visited   = visitedCell(cell);
    northDoor = (cell>>3)&0b1;
    eastDoor 	= (cell>>2)&0b1;
    southDoor = (cell>>1)&0b1;
    westDoor 	= (cell)&0b1;

    if(visited==0) {
      //Visited cell
      maze->maze_array[minotaur.x][minotaur.y] = cell | 0b10000;
      cell = maze->maze_array[minotaur.x][minotaur.y];

    }
    if(!rightWay){
      //Let's assume it's the right way
      maze->maze_array[minotaur.x][minotaur.y] = cell | 0b1000000000000000;
      cell = maze->maze_array[minotaur.x][minotaur.y];
    }
    if(northDoor==0 && northCell!=32 && visitedCell(northCell)==0) {
      move_minotaur(maze, 'N');
    } else if (eastDoor==0 && eastCell!=32 && visitedCell(eastCell)==0) {
      move_minotaur(maze, 'E');
    } else if (southDoor==0 && southCell!=32 && visitedCell(southCell)==0) {
      move_minotaur(maze, 'S');
    } else if (westDoor==0 && westCell!=32 && visitedCell(westCell)==0) {
      move_minotaur(maze, 'W');
    } else {
      //this ain't the right way
      maze->maze_array[minotaur.x][minotaur.y] = cell & 0b0111111111111111;
      //move back to the last right way cell
      if(northDoor==0 && northCell!=32 && on_the_right_way(northCell)==1) {
        move_minotaur(maze, 'N');
      } else if (eastDoor==0 && eastCell!=32 && on_the_right_way(eastCell)==1) {
        move_minotaur(maze, 'E');
      } else if (southDoor==0 && southCell!=32 && on_the_right_way(southCell)==1) {
        move_minotaur(maze, 'S');
      } else if (westDoor==0 && westCell!=32 && on_the_right_way(westCell)==1) {
        move_minotaur(maze, 'W');
      }
      /*
      sleep(2);
      printf("\033[3;10H                                               ");
      printf("\033[3;10H    The minotaur is blocked. Rage quiting !    ");
      printf("\033[3;10H                                               ");
      exit(12);
      */
    }
    //minautor is on the out cell
    minotaur = maze->minotaur;
    win = (minotaur.x==out.x)&&(minotaur.y==out.y);
  }
  return;
}

void smart_mode(MAZE* maze) {
  return;
}
