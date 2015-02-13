#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "grid.h"
#include <ncurses.h>
#include <curses.h>

static void display_grid(grid g,int *ch){
  initscr();
  clear();
  newwin(0,0,40,40);
  keypad(stdscr, TRUE);
  int x=2;
  int y=1;
  for (int i=0;i<5;i++){
    mvhline(x,2,0,27);
    x+=4;
    mvvline(3,y,0,15);
    y+=7;
  }

  x=4; 
  for (int i=0;i<GRID_SIDE;i++){
    y=4;
    for (int j=0;j<GRID_SIDE;j++){
      char buff[5];
      sprintf(buff, "%d", get_tile(g,i,j));
      mvprintw(x,y,buff);
      y+=7;
   }
    x+=4;
  }
  mvprintw(1,13,"2048");
  char score[10];
  sprintf(score, "%lu", grid_score(g));
  mvprintw(19,5,"Score: ");
  mvprintw(19,12,score);
  refresh();
  cbreak();   
  *ch=getch();
  endwin();    
}

int main(int argc,char **argv){
  grid g = new_grid();
  int ch=0;
  display_grid(g,&ch);
  while(!game_over(g)){
    switch(ch){
    case KEY_UP:
      play(g,UP);
      break;
    case KEY_DOWN:
      play(g,DOWN);
      break;
    case KEY_RIGHT:
      play(g,RIGHT);
      break;
    case KEY_LEFT:
      play(g,LEFT);
      break;
    }
    display_grid(g,&ch);
  }

  return EXIT_SUCCESS;
}
