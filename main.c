#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "display.h"

int main(int argc, char* argv[]){
  int choice = 0;
  system("clear");
  printf("\033[1;1H");
  choice = launch_menu();
  return 0;
}
