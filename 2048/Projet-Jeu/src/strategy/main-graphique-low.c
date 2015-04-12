#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <grid.h>
#include <strategy.h>
#include <math.h>

static void display_grid(grid g);
static void display_gameOver(bool *continuer, int *reponse_valide);
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
      display_gameOver(&continuer, &reponse_valide);
    endwin();

  }
  str->free_strategy(str);
  return EXIT_SUCCESS;
}


static void display_gameOver(bool *continuer, int *reponse_valide){
  keypad(stdscr, TRUE);
  int ch = 0;
  mvprintw(9, 45, "GAME OVER");
  mvprintw(11, 36, "Voulez-vous rejouer? y or n ? ");
  refresh();
  cbreak();
  ch = getch();
  if(ch == 110){
    *continuer = false; // Arrete le programme et sort de la boucle
    *reponse_valide = 1;
  }
  if(ch == 121)
    *reponse_valide = 1; // Relance le programme du début
  endwin();
}

static void display_grid(grid g){
  initscr();
  clear();
  keypad(stdscr, TRUE);
  // Mise en forme de la grille
  int x = 3;
  int y = 2;
  for(int i = 0; i < 5; i++){
  mvhline(x, 3, ACS_HLINE, 31);
    x += 4;
  }
  for(int i = 0; i < 5; i++){
    mvvline(4, y, ACS_VLINE, 15);
    y += 8;
  }
  start_color();
  // Définition des couleurs (rouge, vert, jaune, bleu foncé, violet, bleu turquoise, blanc)
  for(int i = 1; i < 8; i++){
    init_pair(i, i, 0);
  }
  // Mise des valeurs dans la grille
  x = 5;
  int t[] = {1,3,2,6,4,5,7};
  for(int i = 0; i < GRID_SIDE; i++){
    y = 5;
    for(int j = 0; j < GRID_SIDE; j++){
      if(get_tile(g, j, i) != 0){
	attron(COLOR_PAIR(t[(get_tile(g, j, i) - 1) % 7]));
	// Récupération d'un nombre et de la coloration
	char buff[5];
	sprintf(buff, "%d", (unsigned int)pow(2, get_tile(g, j, i)));
	mvprintw(x, y, buff);
	attroff(COLOR_PAIR(1));
      }
      y += 8;
    }
    x += 4;
  }
  mvprintw(1, 15, "2048");
  mvprintw(21, 3, "Recommencer Partie / Quitter Partie (r / q)");
  refresh();
}
