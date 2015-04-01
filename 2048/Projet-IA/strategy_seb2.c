
#include "strategy.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include <assert.h>
#include "../Projet-Jeu/src/grid.h"
#include <ncurses.h>
#include <curses.h>

#define MONO_WEIGHT 15
#define MONO_POW 4
#define SMOOTH_WEIGHT 20
#define EMPTY_WEIGHT 10
#define SCORE_WEIGHT 0.1
#define MERGES_WEIGHT 70

//static bool objectif_atteint(grid g);
static long maximum_tile(grid g);
long int note_grid (grid g);
long int generation_case_empty(grid g, int nombre);
long int move_every_dir(grid g, int nombre);
static int empty_tiles(grid g);
static int monotonicity(grid g);
static int smoothness(grid g);
static bool max_in_corner(grid g);

long int max(long int t,long int s){
  if (t>s)
    return t;
  return s;
}
dir meilleur_direction(grid g,dir d,dir d1,int nombre){
  if (!can_move(g,d1))
    return d;
  if (!can_move(g,d))
    return d1;
  grid g_copy = new_grid();
  copy_grid(g,g_copy);
  grid g_copy1 = new_grid();
  copy_grid(g,g_copy1);
  do_move(g_copy,d);
  do_move(g_copy1,d1);
  long int noteD = generation_case_empty(g_copy, nombre);
  long int noteD1 = generation_case_empty(g_copy1, nombre);
  delete_grid(g_copy);
  delete_grid(g_copy1);
  if (noteD>noteD1)
    return d;
  return d1;
  
}
void free_memless_strat (strategy strat)
{
  free (strat);
}

dir strategy_seb(strategy str, grid g){
  int nombre = 4;
  return meilleur_direction(g,meilleur_direction(g,UP,DOWN,nombre),meilleur_direction(g,LEFT,RIGHT,nombre),nombre);
}

long int generation_case_empty(grid g, int nombre){
  if (nombre == 0)
    return note_grid(g);
  int cpt = 0;
  long int note = 0;
  for(int y = 0; y < GRID_SIDE; y++)
    for(int x = 0; x < GRID_SIDE; x++)
      if(get_tile(g,x,y) == 0){
	set_tile(g,x,y,1);
	note +=(long int) 9*move_every_dir(g,nombre-1);
	set_tile(g,x,y,2);
	note +=(long int) move_every_dir(g,nombre-1);
	set_tile(g,x,y,0);
	cpt+=10;
      }
  if (cpt == 0)
    return 0;
  return (long int) (note /cpt);
}

long int move_every_dir(grid g, int nombre){
  if (nombre == 0){
    grid g_copy = new_grid();
    long int note =0;
    for (int i = UP; i<=RIGHT ; i++){

      copy_grid(g,g_copy);
      if (can_move(g,i)){
	do_move(g,i);
	note +=note_grid(g);
      }
      delete_grid(g_copy);
      return (long int)note/4;
    }
  }
   
  grid g_copy = new_grid();
  long int *tab = malloc(4*sizeof(long int));
  int indice = 0;
  long int note = 0;
  for (int i = UP; i<=RIGHT ; i++){
    copy_grid(g,g_copy);
    if (can_move(g,i)){
      do_move(g,i);
      note = generation_case_empty(g,nombre-1);
      indice ++;
    }
  }
  long int maxi =  max(max(tab[0],tab[1]),max(tab[2],tab[3]));
  free(tab);
  delete_grid(g_copy);
  return note;
}
    
/* long int note_grid (grid g){   */
/*   long int cpt = 0; */
/*   int cpt_case_empty = 0; */

/*   // ajoute cpt la somme de toute les cases et compte le nombre de case vide */
/*   for(int y = 0; y < GRID_SIDE; y++){ */
/*     for(int x = 0; x < GRID_SIDE; x++){ */
/*       cpt += get_tile(g,x,y); */
/*       if(get_tile(g,x,y) == 0){ */
/* 	cpt_case_empty++; */
/*       } */
/*     } */
/*   } */

/*   int max = (int)maximum_tile(g); */

/*   if( get_tile(g, 0, 0) == max || get_tile(g, GRID_SIDE -1 , 0) == max || get_tile(g, 0, GRID_SIDE-1) == max || get_tile(g, GRID_SIDE - 1, GRID_SIDE - 1) == max )     cpt += 500;  */
/*   if (get_tile(g,0,0) == max) */
/*     cpt+=1000; */
/*   if (get_tile(g,1,0)  > get_tile(g, 2,0) ) */
/*     cpt+=10; */
/*   int cpt_changement_sign = 0;  */
/*   for(int y = 0; y < GRID_SIDE; y++) */
/*     for(int x = 0; x < GRID_SIDE-1; x++) */
/*       if(get_tile(g,x,y) < get_tile(g,x+1,y)) */
/* 	cpt_changement_sign++; */
      
/*   cpt -= 100*cpt_changement_sign; */
/*   cpt += 10*grid_score(g); */
/*   cpt += 1000*cpt_case_empty; */
/*   return cpt; */
/* } */
  

long int note_grid(grid g){
  long int mono;
  if(max_in_corner(g))
    mono =(long int) monotonicity(g)+1/(MONO_WEIGHT*MONO_POW);
  else
    mono = (long int)monotonicity(g)/MONO_WEIGHT;
  long int smooth =(long int) smoothness(g)*SMOOTH_WEIGHT;
  long int empty = (long int)empty_tiles(g)*EMPTY_WEIGHT;
  long int score = (long int) grid_score(g) * SCORE_WEIGHT;
  return (long int) mono + smooth + empty + score;
}


static int empty_tiles(grid g){
  int nb = 0;
  for(int i = 0; i<4;i++){
    for(int j = 0;j<4;j++){
      if(get_tile(g,i,j) == 0)
	nb += 1;
    }
  }
  return nb;
}



static int monotonicity(grid g){
  int variation_sign;
  int compteur = 0;
  if(get_tile(g,0,0)>get_tile(g,0,1))
    variation_sign = 1;
  else
    variation_sign = 0;
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_tile(g, i, j) < get_tile(g, i, j+1) && variation_sign == 1){
	variation_sign = 0;
	compteur += 1;
      }
      if(get_tile(g, i, j) > get_tile(g, i, j+1) && variation_sign == 0){
	variation_sign = 1;
	compteur += 1;
      }
      if(get_tile(g, j, i) < get_tile(g, j+1, i) && variation_sign == 1){
	variation_sign = 0;
	compteur += 1;
      }
      if(get_tile(g, j, i) > get_tile(g, j+1, i) && variation_sign == 0){
	variation_sign = 1;
	compteur += 1;
      }
    }
  }  
  return compteur*(-1);
}

static int smoothness(grid g){
  int sum = 0;
  for(int i = 0;i<3;i++){
    for(int j = 0;j<3;j++){
      if(get_tile(g,i,j)>=get_tile(g,i,j))
	sum+=(int)get_tile(g,i,j)-(int)(get_tile(g,i,j+1));
      else
	sum+=(int)get_tile(g,i,j+1)-(int)(get_tile(g,i,j));
      if(get_tile(g,j,i)>=get_tile(g,j+1,i))
	sum+=(int)get_tile(g,j,i)-(int)(get_tile(g,j+1,i));
      else
	sum+=(int)get_tile(g,j+1,i)-(int)(get_tile(g,j,i));
    }
  }
  return sum;
}

bool max_in_corner(grid g){
  long max_tile = maximum_tile(g);
  return get_tile(g, 0, 0) == max_tile || get_tile(g, 3, 0) == max_tile || get_tile(g, 0, 3) == max_tile || get_tile(g, 3, 3) == max_tile;
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

int main (int argc, char **argv){

  int n = 10;
  int nb_lance = n;
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
      if(can_move(g,d))
	 play(g, d);
    }
    printf("max %lu",maximum_tile(g));
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
    if(maximum_tile(g) == 11)
      cpt_4096 += 1;
    if(maximum_tile(g) == 11)
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

/*
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
	direction = strategy_seb(seb,g,1);
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


*/
