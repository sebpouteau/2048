#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "../src/grid.h"
#include <ncurses.h>
#include <curses.h>
#include <time.h>

static void display_grid(grid g);
static void display_gameOver(bool *continuer, int *reponse_valide);
static unsigned long int read_highscore();
static void write_highscore(unsigned long int score);


int main(int argc, char *argv[]){
  keypad(stdscr, TRUE);
  bool continuer = true;
  srand(time(NULL));
  while(continuer){
    grid g = new_grid();
    add_tile(g);
    add_tile(g);
    int ch = 0;
    display_grid(g);
    int reponse_valide = 0;
    while(!game_over(g) && reponse_valide == 0){
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
    while(reponse_valide == 0 && continuer == true)
      display_gameOver(&continuer, &reponse_valide);
    endwin();    
  }
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
      if(get_tile(g, i, j) != 0){
	attron(COLOR_PAIR(t[(get_tile(g, i, j) - 1) % 7]));
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
  mvprintw(5, 36, "Score: %lu ", grid_score(g));
  unsigned long int read_highscore();
  if(grid_score(g) > read_highscore()){
    write_highscore(grid_score(g));
  }
  mvprintw(6, 36, "High Score: %lu ", read_highscore());
  mvprintw(21, 3, "Recommencer Partie / Quitter Partie (r / q)");
  refresh();
}

static unsigned long int read_highscore(){
  unsigned long int score[1];
  FILE *highscore = NULL;
  highscore = fopen("../ncurses/highscore_ncurses.txt","r");
  if(highscore == NULL)
    return 0;
  else{
    fscanf(highscore, "%lu", &score[0]);
    fclose(highscore);
  }
  return score[0];
}

static void write_highscore(unsigned long int score){
  FILE *highscore = NULL;
  highscore = fopen("../ncurses/highscore_ncurses.txt", "w");
  fprintf(highscore, "%lu", score);
  fclose(highscore);
}
