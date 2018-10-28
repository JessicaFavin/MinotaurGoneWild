#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>
#include "maze.h"
#include "display.h"

#define MIN(a,b) (((a)<(b))?(a):(b))

#define MAX(a,b) (((a)>(b))?(a):(b))


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



void move_minotaur(MAZE* maze, int direction, int wait) {
  if(wait)
    sleep(1);

  erase_minotaur(maze);
  POS minotaur = maze->minotaur;
  unsigned short cell = maze->maze_array[minotaur.x][minotaur.y];
  int northDoor, eastDoor, southDoor, westDoor;
  northDoor = (cell>>3)&0b1;
  eastDoor 	= (cell>>2)&0b1;
  southDoor = (cell>>1)&0b1;
  westDoor 	= (cell)&0b1;
  switch(direction) {
    case 'N':
      if (minotaur.x>0 && !northDoor)
        maze->minotaur.x -= 1;
      break;
    case 'E':
      if (minotaur.y<maze->col-1 && !eastDoor)
        maze->minotaur.y += 1;
      break;
    case 'S':
      if (minotaur.x<maze->line-1 && !southDoor)
        maze->minotaur.x += 1;
      break;
    case 'W':
      if (minotaur.y>0 && !westDoor)
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

int player_actions(char c, MAZE* maze){
    int b = 1;
    POS minotaur = maze->minotaur;
    if((c=='d'||c=='D') && minotaur.y<maze->col-1){
        move_minotaur(maze, 'W', 0);
    }
    if((c=='q'||c=='C') && minotaur.y>0){
        move_minotaur(maze, 'E', 0);
    }
    if((c=='A' || c=='z') && minotaur.x>0){
      move_minotaur(maze, 'N', 0);

    }
    if((c=='B' || c=='s')  && minotaur.x<maze->line-1){
      move_minotaur(maze, 'S', 0);
    }
    if(c=='l'){
        b = 0;
    }
    return b;
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
      move_minotaur(maze, 'N', 1);
    } else if (eastDoor==0 && eastCell!=32 && visitedCell(eastCell)==0) {
      move_minotaur(maze, 'E', 1);
    } else if (southDoor==0 && southCell!=32 && visitedCell(southCell)==0) {
      move_minotaur(maze, 'S', 1);
    } else if (westDoor==0 && westCell!=32 && visitedCell(westCell)==0) {
      move_minotaur(maze, 'W', 1);
    } else {
      //this ain't the right way
      maze->maze_array[minotaur.x][minotaur.y] = cell & 0b0111111111111111;
      //move back to the last right way cell
      if(northDoor==0 && northCell!=32 && on_the_right_way(northCell)==1) {
        move_minotaur(maze, 'N', 1);
      } else if (eastDoor==0 && eastCell!=32 && on_the_right_way(eastCell)==1) {
        move_minotaur(maze, 'E', 1);
      } else if (southDoor==0 && southCell!=32 && on_the_right_way(southCell)==1) {
        move_minotaur(maze, 'S', 1);
      } else if (westDoor==0 && westCell!=32 && on_the_right_way(westCell)==1) {
        move_minotaur(maze, 'W', 1);
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
  int win = 0;
  int northDoor, eastDoor, southDoor, westDoor, visited, rightWay;
  unsigned short cell, northCell, eastCell, southCell, westCell;
  display_maze(maze);
  POS minotaur = maze->minotaur;
  POS out = maze->out;
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

    double northDistance = 300000;
    double eastDistance = 300000;
    double southDistance = 300000;
    double westDistance = 300000;

    if(northDoor==0 && northCell!=32 && visitedCell(northCell)==0) {
       northDistance = sqrt(pow(minotaur.x-1-out.x,2)+pow(minotaur.y-out.y,2));
    }
    if (eastDoor==0 && eastCell!=32 && visitedCell(eastCell)==0) {
       eastDistance =  sqrt(pow(minotaur.x-out.x,2)+pow(minotaur.y+1-out.y,2));
    }
    if (southDoor==0 && southCell!=32 && visitedCell(southCell)==0) {
       southDistance = sqrt(pow(minotaur.x+1-out.x,2)+pow(minotaur.y-out.y,2));
    }
    if (westDoor==0 && westCell!=32 && visitedCell(westCell)==0) {
       westDistance =  sqrt(pow(minotaur.x-out.x,2)+pow(minotaur.y-1-out.y,2));
    }

    if(northDistance!=300000 && northDistance == MIN(northDistance, MIN(eastDistance, MIN(southDistance, westDistance)))) {
      move_minotaur(maze, 'N', 1);
    } else if (eastDistance!=300000 && eastDistance == MIN(eastDistance, MIN(southDistance, MIN(westDistance, northDistance)))) {
      move_minotaur(maze, 'E', 1);
    } else  if (southDistance!=300000 && southDistance == MIN(southDistance, MIN(westDistance, MIN(northDistance, eastDistance)))) {
      move_minotaur(maze, 'S', 1);
    } else if (westDistance!=300000 && westDistance == MIN(westDistance, MIN(northDistance, MIN(eastDistance, southDistance)))) {
      move_minotaur(maze, 'W', 1);
    }

    //Cul-de-sac
    if((northDoor || northCell==32 || visitedCell(northCell)) && (eastDoor || eastCell==32 || visitedCell(eastCell))
    && (southDoor || southCell==32 || visitedCell(southCell)) && (westDoor || westCell==32 || visitedCell(westCell))) {
      //this ain't the right way
      maze->maze_array[minotaur.x][minotaur.y] = cell & 0b0111111111111111;
      //move back to the last right way cell
      if(northDoor==0 && northCell!=32 && on_the_right_way(northCell)==1) {
        move_minotaur(maze, 'N', 1);
      } else if (eastDoor==0 && eastCell!=32 && on_the_right_way(eastCell)==1) {
        move_minotaur(maze, 'E', 1);
      } else if (southDoor==0 && southCell!=32 && on_the_right_way(southCell)==1) {
        move_minotaur(maze, 'S', 1);
      } else if (westDoor==0 && westCell!=32 && on_the_right_way(westCell)==1) {
        move_minotaur(maze, 'W', 1);
      }
    }

    //minautor is on the out cell
    minotaur = maze->minotaur;
    win = (minotaur.x==out.x)&&(minotaur.y==out.y);
  }
  return;
}

void play_mode(MAZE* maze) {
  int win = 0;
  POS out = maze->out;
  POS minotaur;
  display_maze(maze);

  while(win==0){
    char c = key_pressed();
    player_actions(c, maze);
    minotaur = maze->minotaur;
    win = (minotaur.x==out.x)&&(minotaur.y==out.y);
  }

  return;

}
