
#include "strategy.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include <assert.h>
//#include "grid.h"
#include <ncurses.h>
#include <curses.h>

//static bool objectif_atteint(grid g);
static long maximum_tile(grid g);
long int note_grid (grid g, dir t);


dir meilleur_direction(grid g,dir d,dir d1){
  if (note_grid(g,d)>note_grid(g,d1))
    return d;
  return d1;
  
}
void free_memless_strat (strategy strat)
{
  free (strat);
}


/* if (can_move(g,UP)){return UP;} */
  /* else if (can_move(g,LEFT)){return LEFT;} */
  /* else if (can_move(g,DOWN)){return DOWN;} */
  /* else if (can_move(g,RIGHT)){return RIGHT;} */
  /* return UP; */
dir strategy_seb(strategy str, grid g){
  /* grid g1 = new_grid(); */
  /* long int up; */
  /* long int down; */
  /* long int left; */
  /* long int right; */

  /* copy_grid(g,g1); */
  /* up = note_grid(g1,UP); */

  /* copy_grid(g,g1); */
  /* down = note_grid(g1,DOWN); */
  
  /* copy_grid(g,g1); */
  /* right = note_grid(g1,RIGHT); */
  
  /* copy_grid(g,g1); */
  /* left = note_grid(g1,LEFT); */
  
  //delete_grid(g1);
  return meilleur_direction(g,meilleur_direction(g,UP,DOWN),meilleur_direction(g,LEFT,RIGHT));
}

long int note_grid (grid g, dir t){
  if (!can_move(g,t))
    return 0;
  grid g_copy = new_grid();
  copy_grid(g,g_copy);
  
  do_move(g_copy,t);
  int cpt = 0;
  int cpt_case_empty = 0;
  for(int y = 0; y < GRID_SIDE; y++){
    for(int x = 0; x < GRID_SIDE; x++){
      cpt += get_tile(g_copy,x,y);
      if(get_tile(g_copy,x,y) == 0)
	cpt_case_empty++;
    }
  }
  int max = (int)maximum_tile(g_copy);
  if( get_tile(g_copy, 0, 0) == max || get_tile(g_copy, GRID_SIDE -1 , 0) == max || get_tile(g_copy, 0, GRID_SIDE-1) == max || get_tile(g_copy, GRID_SIDE - 1, GRID_SIDE - 1) == max )
    cpt += 100;

  cpt += 2*grid_score(g_copy);
  delete_grid(g_copy);
  cpt += 10*cpt_case_empty;
  return cpt;
}
  

strategy init_Structure (){
  strategy str = malloc (sizeof(struct strategy_s));
  str->name = "strategy_seb";
  str->mem = NULL;
  str->free_strategy = free_memless_strat;
  str->play_move = strategy_seb;
  return str;
}

static long maximum_tile(grid g){
  long max_tile = 2;
  for(int i = 0; i<GRID_SIDE; i++){
    for(int j = 0; j< GRID_SIDE; j++){
      if(get_tile(g, i, j)>max_tile)
	max_tile = get_tile(g, i, j);
    }
  }
  return max_tile;
 }
/* int main (int argc, char **argv){ */

/*   int n = 1000; */
/*   int nb_lance = n; */
/*   int cpt_16 = 0; */
/*   int cpt_32 = 0; */
/*   int cpt_64 = 0; */
/*   int cpt_128 = 0; */
/*   int cpt_256 = 0; */
/*   int cpt_512 = 0; */
/*   int cpt_1024 = 0; */
/*   int cpt_2048 = 0; */
	
/*   srand(time(NULL)); */
/*   strategy seb = init_Structure(); */
/*   while(n > 0){ */
/*     grid g = new_grid(); */
/*     add_tile(g); */
/*     add_tile(g); */
    
/*     while(!game_over(g)){ */
/*       play(g, strategy_seb(seb,g)); */
/*     } */
    
/*     if(maximum_tile(g) == 4) */
/*       cpt_16 += 1; */
/*     if(maximum_tile(g) == 5) */
/*       cpt_32 += 1; */
/*     if(maximum_tile(g) == 6) */
/*       cpt_64 += 1; */
/*     if(maximum_tile(g) == 7) */
/*       cpt_128 += 1; */
/*     if(maximum_tile(g) == 8) */
/*       cpt_256 += 1; */
/*     if(maximum_tile(g) == 9) */
/*       cpt_512 += 1; */
/*     if(maximum_tile(g) == 10) */
/*       cpt_1024 += 1; */
/*     if(maximum_tile(g) == 11) */
/*       cpt_1024 += 1; */
/*     delete_grid(g); */
/*     n -= 1; */
/*   } */
/*   printf("\n --------------- \n"); */
/*   printf("Sur %d lancés : \n\n", nb_lance); */
/*   printf("Nombre de fois 16 : %d\n", cpt_16); */
/*   printf("Nombre de fois 32 : %d\n", cpt_32); */
/*   printf("Nombre de fois 64 : %d\n", cpt_64); */
/*   printf("Nombre de fois 128 : %d\n", cpt_128); */
/*   printf("Nombre de fois 256 : %d\n", cpt_256); */
/*   printf("Nombre de fois 512 : %d\n", cpt_512); */
/*   printf("Nombre de fois 1024 : %d\n", cpt_1024); */
/*   printf("Nombre de fois 2048 : %d\n", cpt_2048); */
/* } */


static void display_grid(grid g);
static void display_gameOver(bool *continuer, int *reponse_valide);


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
      strategy seb = init_Structure();
      dir direction;
      ch=getch();
      switch(ch){
      case KEY_UP:
	direction = strategy_seb(seb,g);
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

