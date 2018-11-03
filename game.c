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
}

void move_minotaur_nd(MAZE* maze, int direction) {
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
    if((c=='d'||c=='D') && minotaur.y>0){
        move_minotaur(maze, 'W', 0);
    }
    if((c=='q'||c=='C') && minotaur.y<maze->col-1){
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

int depth_search(MAZE* maze, int display) {
  POS out = maze->out;
  POS minotaur = maze->minotaur;
  if(display) {
    display_minotaur(maze);
    sleep(1);
  }

  if(minotaur.x==out.x && minotaur.y==out.y){
    //found the way out
    return 1;
  }
  int northDoor, eastDoor, southDoor, westDoor, visited, rightWay;
  unsigned short cell, northCell, eastCell, southCell, westCell;
  cell = maze->maze_array[minotaur.x][minotaur.y];
  rightWay  = on_the_right_way(cell);
  visited   = visitedCell(cell);
  northDoor = (cell>>3)&0b1;
  eastDoor 	= (cell>>2)&0b1;
  southDoor = (cell>>1)&0b1;
  westDoor 	= (cell)&0b1;
  if (minotaur.x>0) {
    northCell = maze->maze_array[minotaur.x-1][minotaur.y];
  }
  if (minotaur.y<maze->col-1) {
    eastCell = maze->maze_array[minotaur.x][minotaur.y+1];
  }
  if (minotaur.x<maze->line-1) {
    southCell = maze->maze_array[minotaur.x+1][minotaur.y];
  }
  if (minotaur.y>0) {
    westCell = maze->maze_array[minotaur.x][minotaur.y-1];
  }
  //Visited cell
  maze->maze_array[minotaur.x][minotaur.y] = cell | 0b10000;
  cell = maze->maze_array[minotaur.x][minotaur.y];

  if(!rightWay){
    //Let's assume it's the right way
    maze->maze_array[minotaur.x][minotaur.y] = cell | 0b1000000000000000;
    cell = maze->maze_array[minotaur.x][minotaur.y];
  }
  int foundOut = 0;
  //for all cells around
  if(!northDoor && !visitedCell(northCell)) {
    if(display)
      erase_minotaur(maze);
    move_minotaur_nd(maze,'N');
    foundOut = depth_search(maze, display);
    if(foundOut==0) {
      if(display)
        erase_grass(maze);
      move_minotaur_nd(maze,'S');
      if(display){
        display_minotaur(maze);
        sleep(1);
      }
    } else {
      return 1;
    }
  }
  if(!eastDoor && !visitedCell(eastCell)) {
    if(display)
      erase_minotaur(maze);
    move_minotaur_nd(maze,'E');
    foundOut = depth_search(maze, display);
    if(foundOut==0) {
      if(display)
        erase_grass(maze);
      move_minotaur_nd(maze,'W');
      if(display){
        display_minotaur(maze);
        sleep(1);
      }
    } else {
      return 1;
    }
  }
  if(!southDoor && !visitedCell(southCell)) {
    if(display)
      erase_minotaur(maze);
    move_minotaur_nd(maze,'S');
    foundOut = depth_search(maze, display);
    if(foundOut==0) {
      if(display)
        erase_grass(maze);
      move_minotaur_nd(maze,'N');
      if(display){
        display_minotaur(maze);
        sleep(1);
      }
    } else {
      return 1;
    }
  }
  if(!westDoor && !visitedCell(westCell)) {
    if(display)
      erase_minotaur(maze);
    move_minotaur_nd(maze,'W');
    foundOut = depth_search(maze, display);
    if(foundOut==0) {
      if(display)
        erase_grass(maze);
      move_minotaur_nd(maze,'E');
      if(display){
        display_minotaur(maze);
        sleep(1);
      }
    } else {
      return 1;
    }
  }
  return 0;
}

void berserk_mode(MAZE* maze) {
  display_maze(maze);
  int win = depth_search(maze, 1);
  system("play -q -v 0.99 ./Ressources/Sounds/Moo.mp3 2> /dev/null &");
}

int valid_labyrinth(MAZE* maze) {
  int valid = depth_search(maze, 0);
  return valid;

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
    }
    if (minotaur.y<maze->col-1) {
      eastCell = maze->maze_array[minotaur.x][minotaur.y+1];
    }
    if (minotaur.x<maze->line-1) {
      southCell = maze->maze_array[minotaur.x+1][minotaur.y];
    }
    if (minotaur.y>0) {
      westCell = maze->maze_array[minotaur.x][minotaur.y-1];
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

    if(northDoor==0 && visitedCell(northCell)==0) {
       northDistance = sqrt(pow(minotaur.x-1-out.x,2)+pow(minotaur.y-out.y,2));
    }
    if (eastDoor==0 && visitedCell(eastCell)==0) {
       eastDistance =  sqrt(pow(minotaur.x-out.x,2)+pow(minotaur.y+1-out.y,2));
    }
    if (southDoor==0 && visitedCell(southCell)==0) {
       southDistance = sqrt(pow(minotaur.x+1-out.x,2)+pow(minotaur.y-out.y,2));
    }
    if (westDoor==0 && visitedCell(westCell)==0) {
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
    if((northDoor || visitedCell(northCell)) && (eastDoor || visitedCell(eastCell))
    && (southDoor || visitedCell(southCell)) && (westDoor || visitedCell(westCell))) {
      //this ain't the right way
      maze->maze_array[minotaur.x][minotaur.y] = cell & 0b0111111111111111;
      //move back to the last right way cell
      if(northDoor==0 && on_the_right_way(northCell)==1) {
        move_minotaur(maze, 'N', 1);
      } else if (eastDoor==0 && on_the_right_way(eastCell)==1) {
        move_minotaur(maze, 'E', 1);
      } else if (southDoor==0 && on_the_right_way(southCell)==1) {
        move_minotaur(maze, 'S', 1);
      } else if (westDoor==0 && on_the_right_way(westCell)==1) {
        move_minotaur(maze, 'W', 1);
      }
    }

    //minautor is on the out cell
    minotaur = maze->minotaur;
    win = (minotaur.x==out.x)&&(minotaur.y==out.y);
  }
  system("play -q -v 0.99 ./Ressources/Sounds/Moo.mp3 2> /dev/null &");
  return;
}

void play_mode(MAZE* maze) {
  int win = 0;
  POS out = maze->out;
  POS minotaur;
  display_maze(maze);
  while(win==0){
    char c = key_pressed();
    if(c=='l'){
      return;
    }
    player_actions(c, maze);
    minotaur = maze->minotaur;
    if(c == 0){
      win = (minotaur.x==out.x)&&(minotaur.y==out.y);
    }
  }
  system("play -q -v 0.99 ./Ressources/Sounds/Moo.mp3 2> /dev/null &");
  return;

}
