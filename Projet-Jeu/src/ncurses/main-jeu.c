#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <grid.h>
#include <ncurses.h>
#include <curses.h>
#include <gridNcurses.h>
#include <time.h>

/**
 * \file main-jeu.c
 * \brief fichier permetant le jeu en ncurses
 **/
 
#define KEY_Y 110
#define KEY_Q 113
#define KEY_N 121
#define KEY_R 114

int main(int argc, char *argv[]){
  keypad(stdscr, TRUE);
  bool continue_game = true;
  srand(time(NULL));
  while(continue_game){
    grid g = new_grid();
    add_tile(g);
    add_tile(g);
    int ch = 0;
    display_grid(g);
    int valid_answer = 0;
    while(!game_over(g) && valid_answer == 0){
      ch=getch();
      switch(ch){
      case KEY_UP:
	if (can_move(g, UP))
	  play(g, UP);
	break;
      case KEY_DOWN:
	if (can_move(g, DOWN))
	  play(g, DOWN);
	break;
      case KEY_RIGHT:
	if (can_move(g, RIGHT))
	  play(g, RIGHT);
	break;
      case KEY_LEFT:
	if (can_move(g, LEFT))
	  play(g, LEFT);
	break;
      case KEY_Q: // "q" pour quitter
	continue_game = false;
	valid_answer = 1;
	break;
      case KEY_R: // "r" pour rejouer
	valid_answer = 1;
	break;
      }
      display_grid(g);
    }
    while(valid_answer == 0 && continue_game == true)
      display_game_over(&continue_game, &valid_answer);
    delete_grid(g);
    endwin();    
  }
  return EXIT_SUCCESS;
}

