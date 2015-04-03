
#include "strategy.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include <assert.h>
#include "../Projet-Jeu/src/grid.h"
#include <ncurses.h>
#include <curses.h>

#define PROFONDEUR 3
#define NOMBRE_TEST 1

//static bool objectif_atteint(grid g);
static long maximum_tile(grid g);
long int note_grid (grid g);
static long int repetition_grid(grid g, int nombre, dir *d);
long int maximum(long int l,long int l1,dir d, dir d1, dir *d2);

dir meilleur_direction(grid g){
  int nombre = PROFONDEUR;
  dir d = UP;
  repetition_grid(g,nombre,&d);
  return d;
 }

dir strategy_seb(strategy str, grid g){
  return meilleur_direction(g);
  
}



static long int repetition_grid(grid g, int nombre,dir *d){
  if (game_over(g)){
    return 0;
  }
  if (nombre == 0){
    return note_grid(g);    
  }
  long int tab[4];
  grid g_copy = new_grid();
  int indice_tab =-1;
  for (int i = UP; i<=RIGHT ; i++){
    long int note = 0;
    int cpt = 0;
    indice_tab++;
    copy_grid(g,g_copy);
    if ( can_move(g_copy,i)){
      do_move(g_copy,i);
      for(int y = 0; y < GRID_SIDE; y++)
	for(int x = 0; x < GRID_SIDE; x++)
	  if(get_tile(g_copy,x,y) == 0){
	    set_tile(g_copy,x,y,1);
	    note +=(long int) 9*repetition_grid(g_copy,nombre-1, d);
	       set_tile(g_copy,x,y,2);
	       note +=(long int) repetition_grid(g_copy,nombre-1, d);
	       set_tile(g_copy,x,y,0);
	       cpt +=10;    
	  }
   
    if (cpt == 0)
      tab[indice_tab]=-99999999;
    else
      tab[indice_tab]=(long int)note/cpt;
    }  
    else {
      tab[indice_tab] = -99999999999;
    }
  }    
  delete_grid(g_copy);
  dir mdU_L;
  dir mdD_R;
  long int max_UD = maximum(tab[0],tab[1],UP,LEFT,&mdU_L);
  long int max_LR = maximum(tab[2],tab[3],DOWN,RIGHT,&mdD_R);
  return maximum(max_UD,max_LR,mdU_L,mdD_R,d);  
}


long int maximum(long int l,long int l1,dir d, dir d1, dir *d2){
  if ( l >=l1){
    *d2 = d;
    return l;
  }
  else{
    *d2 = d1;
    return l1;
  }
}

long int note_grid (grid g){
  long int cpt = 0;
  int cpt_case_empty = 0;
  for(int y = 0; y < GRID_SIDE; y++){
    for(int x = 0; x < GRID_SIDE; x++){
      cpt += get_tile(g,x,y);
      if(get_tile(g,x,y) == 0){
	cpt_case_empty++;
      }
    }
  }

  int max = (int)maximum_tile(g);
  if( get_tile(g, 0, 0) == max || get_tile(g, GRID_SIDE -1 , 0) == max || get_tile(g, 0, GRID_SIDE-1) == max || get_tile(g, GRID_SIDE - 1, GRID_SIDE - 1) == max )
    cpt += 1000*max;
  if (get_tile(g,0,0) == max)
    cpt+=800*max;
  int cpt_changement_sign = 0;
  for(int y = 0; y < GRID_SIDE; y++)
    for(int x = 0; x < GRID_SIDE-1; x++)
      if(get_tile(g,x,y) < get_tile(g,x+1,y))
	cpt_changement_sign++;
  for(int x = 0; x < GRID_SIDE; x++)
    for(int y = 0; y < GRID_SIDE-1; y++)
      if(get_tile(g,x,y) < get_tile(g,x,y+1))
  	cpt_changement_sign++;
  cpt += 500*max;    
  cpt -= 3000*cpt_changement_sign;
  cpt += 10*grid_score(g);
  
  cpt += 1000*cpt_case_empty;
  return cpt;
}
  


strategy init_Structure (char *str_choose){
  strategy str = malloc (sizeof(struct strategy_s));
  str->name = "strategy_seb";
  str->mem = malloc(4*sizeof(long int));
  str->free_strategy = free_memless_strat;
  str->play_move = strategy_seb;
  return str;
}
 
void free_memless_strat (strategy strat)
{
  free (strat);
}
 
static long maximum_tile(grid g){
  long max_tile = 1;
  for(int i = 0; i<GRID_SIDE; i++){
    for(int j = 0; j< GRID_SIDE; j++){
      if(get_tile(g, i, j)>max_tile)
	max_tile = get_tile(g, i, j);
    }
  }
  return max_tile;
}
/*
int main (int argc, char **argv){

  int n = NOMBRE_TEST;
  int cpt_16 = 0;
  int cpt_32 = 0;
  int cpt_64 = 0;
  int cpt_128 = 0;
  int cpt_256 = 0;
  int cpt_512 = 0;
  int cpt_1024 = 0;
  int cpt_2048 = 0;
  int cpt_4096 = 0;
  int cpt_8192= 0;
	
  srand(time(NULL));
  strategy seb = init_Structure();
  while(n > 0){
    grid g = new_grid();
    add_tile(g);
    add_tile(g);
    
    while(!game_over(g)){
      dir d = strategy_seb(seb,g);
      if (can_move(g,d))
	play(g, d);
    }
    printf("max %lu\n",(long int)pow(2,maximum_tile(g)));
    if(maximum_tile(g) == 4)
      cpt_16 += 1;
    if(maximum_tile(g) == 5)
      cpt_32 += 1;
    if(maximum_tile(g) == 6)
      cpt_64 += 1;
    if(maximum_tile(g) == 7)
      cpt_128 += 1;
    if(maximum_tile(g) == 8)
      cpt_256 += 1;
    if(maximum_tile(g) == 9)
      cpt_512 += 1;
    if(maximum_tile(g) == 10)
      cpt_1024 += 1;
    if(maximum_tile(g) == 11)
      cpt_2048 += 1;
    if(maximum_tile(g) == 12)
      cpt_4096 += 1;
    if(maximum_tile(g) == 13)
      cpt_8192 += 1;
    delete_grid(g);
    n -= 1;
  }
  printf("\n --------------- \n");
  printf("Sur %d lancés : \n\n", nb_lance);
  printf("Nombre de fois 16 : %d\n", cpt_16);
  printf("Nombre de fois 32 : %d\n", cpt_32);
  printf("Nombre de fois 64 : %d\n", cpt_64);
  printf("Nombre de fois 128 : %d\n", cpt_128);
  printf("Nombre de fois 256 : %d\n", cpt_256);
  printf("Nombre de fois 512 : %d\n", cpt_512);
  printf("Nombre de fois 1024 : %d\n", cpt_1024);
  printf("Nombre de fois 2048 : %d\n", cpt_2048);
  printf("Nombre de fois 4096 : %d\n", cpt_4096);
  printf("Nombre de fois 8192 : %d\n", cpt_8192);
}
*/

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
      strategy str = init_Structure(argv[1]);
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


