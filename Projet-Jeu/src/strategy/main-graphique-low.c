#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <grid.h>
#include <strategy.h>
#include <math.h>
#include <gridNcurses.h>
/** 
 * \file main-graphique-low.c
 * \brief fait jouer en graphique l'IA Lente
 */
 


/**
 * \brief Strategie utilisé
 * \return stretegy
 */
strategy A2_emery_gouraud_kirov_pouteau_low();

int main(int argc, char *argv[]){
  keypad(stdscr, TRUE);
  bool continuer = true;
  srand(time(NULL));
  strategy str = A2_emery_gouraud_kirov_pouteau_low();
  while(continuer){
    grid g = new_grid();
    add_tile(g);
    add_tile(g);
    int ch = 0;
    display_grid(g);
    int reponse_valide = 0;
    while(!game_over(g) && reponse_valide == 0){
      dir direction;
      ch=getch();
      switch(ch){
      case KEY_UP:
	direction = str->play_move(str,g);
	if ( can_move(g,direction))
	  play(g,direction);
	break;
      case 113: // "q" pour quitter
	continuer = false;
	reponse_valide = 1;
	break;
     case 114: // "r" pour rejouer
	reponse_valide = 1;
	break;
      }
      display_grid(g);
    }
    delete_grid(g);
    while(reponse_valide == 0 && continuer == true)
      display_game_over(&continuer, &reponse_valide);
    endwin();

  }
  str->free_strategy(str);
  return EXIT_SUCCESS;
}


