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
}

void berserk_mode(MAZE* maze) {
  display_maze(maze);
  while(1){
    move_minotaur(maze, 'E');
    sleep(1);
    move_minotaur(maze, 'S');
    sleep(1);
    move_minotaur(maze, 'W');
    sleep(1);
    move_minotaur(maze, 'N');
    sleep(1);
  }
  return;
}

void smart_mode(MAZE* maze) {
  return;
}
