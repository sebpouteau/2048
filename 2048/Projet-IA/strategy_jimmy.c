#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "strategy.h"
#include "stdlib.h"

void free_memless_strat (strategy strat)
{
  free(strat);
}

static dir strategy_jimmy (grid g);
/*
strategy initStructure(){
  strategy strat = malloc(sizeof(*strat));
  strat->mem = NULL;
  strat->name = "Jimmy";
  strat->play_move = strategy_jimmy;
  strat->free_strategy = free_memless_strat;
}
*/


/* ==================
   == STRATEGIE IA ==
   ================== */

static float poids_fusion (grid g, dir d);
static bool possible_lc (grid g, int i, int j, int a,int b);
static void tri_insertion(float tab[], int taille);
static long maximum_tile (grid g);

static dir strategy_jimmy (grid g){
  float cpt_up = poids_fusion(g, UP);
  float cpt_down = poids_fusion(g, DOWN);
  float cpt_left = poids_fusion(g, LEFT);
  float cpt_right = poids_fusion(g, RIGHT);
 
  if(can_move(g, UP) && can_move(g, LEFT)){
    if(cpt_up >= cpt_left)
      return UP;
    else
      return LEFT;
  }else{
    if(can_move(g, UP) || can_move(g, LEFT)){
      if(can_move(g, UP))
	return UP;
      else
	return LEFT;
    }else{
      if(!possible_lc(g, 0, 0, 1, 0) || !possible_lc(g, 0, 0, 0, 1)){
	if(can_move(g, RIGHT) && !possible_lc(g, 0, 0, 1, 0))
	  return RIGHT;
	else if(can_move(g, DOWN) && !possible_lc(g, 0, 0, 1, 0))
	  return DOWN;
	else{
	  if(cpt_right >= cpt_down)
	    return RIGHT;
	  else
	    return DOWN;
	}
      }else{
	if(cpt_right >= cpt_down)
	  return RIGHT;
	else
	  return DOWN;
      }
    }
  }

  /*
  float tab[4] = {cpt_up, cpt_down, cpt_left, cpt_right};
  tri_insertion(tab, 4);
  for(int i = 0; i < 4; i++){
    if(tab[i] == cpt_up)
      if(can_move(g, UP))
	return UP;
    if(tab[i] == cpt_down)
      if(can_move(g, DOWN))
	return DOWN;
    if(tab[i] == cpt_left)
      if(can_move(g, LEFT))
	return LEFT;
    if(tab[i] == cpt_right)
      if(can_move(g, RIGHT))
	return RIGHT;
  }
  */
}

static float poids_fusion (grid g, dir d){  
  float poids_fusion = 0;
  grid g_copy = new_grid();
  copy_grid(g, g_copy);
  do_move(g_copy, d);
  poids_fusion = grid_score(g_copy) - grid_score(g);

  if(maximum_tile(g) == get_tile(g_copy, 0, 0))
     poids_fusion += get_tile(g_copy, 0, 0);
  if(maximum_tile(g) == get_tile(g_copy, 1, 0))
    poids_fusion += get_tile(g_copy, 1, 0)/2;
  if(maximum_tile(g) == get_tile(g_copy, 0, 1))
     poids_fusion += get_tile(g_copy, 0, 1)/2;
  // -----------
  // On compte le nombre de case vide
  int nb_case_empty = 0;
  for(int i = 0; i < GRID_SIDE; i++){
    for(int j = 0; j < GRID_SIDE; j++){
      if(get_tile(g_copy, i, j) == 0)
	nb_case_empty += 1;
    }
  }
  //----------
  int cpti;
  int cptj;
  for(int i = 0; i < GRID_SIDE; i++){
    cpti = i;
    for(int j = 0; j < GRID_SIDE; j++){
      cptj = j;
      if(get_tile(g_copy, i, j) != 0){
	// On teste si deux cases sur la meme ligne/colonne permettent une fusion.
	// On teste l'apparition d'un 2 ou d'un 4.
	// A completer : le cas où il y a 3 fois le meme chiffre (enlever un cas)
	while(cpti + 1 < GRID_SIDE){
	  cpti += 1;
	  if(get_tile(g_copy, i, j) == get_tile(g_copy, cpti, j))
	    poids_fusion += get_tile(g_copy, i, j)/2;
	  else if(get_tile(g_copy, cpti, j) == 0){
	    if(get_tile(g_copy, i, j) == 2)
	      poids_fusion += (get_tile(g_copy, i, j) * 9 / 10) / nb_case_empty;	    
	    if(get_tile(g_copy, i, j) == 4)
	      poids_fusion += (get_tile(g_copy, i, j) / 10) / nb_case_empty;
	  }
	  else
	    break;
	}
        while(cpti - 1 >= 0){
	  cpti -= 1;
	  if(get_tile(g_copy, i, j) == get_tile(g_copy, cpti, j))
	    poids_fusion += get_tile(g_copy, i, j)/2;
	  else if(get_tile(g_copy, cpti, j) == 0){
	    if(get_tile(g_copy, i, j) == 2)
	      poids_fusion += (get_tile(g_copy, i, j) * 9 / 10) / nb_case_empty;
	    if(get_tile(g_copy, i, j) == 4)
	      poids_fusion += (get_tile(g_copy, i, j) / 10) / nb_case_empty;
	  }
	  else
	    break;
	}
	
	while(cptj + 1 < GRID_SIDE){
	  cptj += 1;
	  if(get_tile(g_copy, i, j) == get_tile(g_copy, i, cptj))
	    poids_fusion += get_tile(g_copy, i, j)/2;
	  else if(get_tile(g_copy, cpti, j) == 0){
	    if(get_tile(g_copy, i, j) == 2)
	      poids_fusion += (get_tile(g_copy, i, j) * 9 / 10) / nb_case_empty;
	    if(get_tile(g_copy, i, j) == 4)
	      poids_fusion += (get_tile(g_copy, i, j) / 10) / nb_case_empty;
	  } 
	  else
	    break;
	}
	
        while(cptj - 1 >= 0){
	  cptj -= 1;
	  if(get_tile(g_copy, i, j) == get_tile(g_copy, i, cptj))
	    poids_fusion += get_tile(g_copy, i, j)/2;
	  else if(get_tile(g_copy, cpti, j) == 0){
	    if(get_tile(g_copy, i, j) == 2)
	      poids_fusion += (get_tile(g_copy, i, j) * 9 / 10) / nb_case_empty;
	    if(get_tile(g_copy, i, j) == 4)
	      poids_fusion += (get_tile(g_copy, i, j) / 10) / nb_case_empty;
	  }
	  else
	    break;
	}
      } // fin du premier if
    } // fin du for(int j = 0; ...)
  } // fin du for(int i = 0; ...)
  return poids_fusion;
}


/* ===========================
   == AFFICHE LES RESULTATS ==
   =========================== */

static long maximum_tile (grid g);

int main (int argc, char **argv){

  int n = 1000;
  int nb_lance = n;
  int cpt_16 = 0;
  int cpt_32 = 0;
  int cpt_64 = 0;
  int cpt_128 = 0;
  int cpt_256 = 0;
  int cpt_512 = 0;
  int cpt_1024 = 0;
  int cpt_2048 = 0;
	
  srand(time(NULL));
 
  while(n > 0){
    grid g = new_grid();
    add_tile(g);
    add_tile(g);
    
    while(!game_over(g)){
      play(g, strategy_jimmy(g));
    }
    
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
      cpt_1024 += 1;
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
}


static long maximum_tile (grid g){
  long max_tile = 2;
  for(int i = 0; i < GRID_SIDE; i++){
    for(int j = 0; j < GRID_SIDE; j++){
      if(get_tile(g, i, j) > max_tile)
	max_tile = get_tile(g, i, j);
    }
  }
  return max_tile;
}


static bool possible_lc (grid g, int i, int j, int a, int b){
  while(i < 4 - a && i >= 0 - a && j < 4 - b && j >= 0 - b){
    if((get_tile(g, i, j) == get_tile(g, i + a, j + b) && get_tile(g, i, j) != 0) || (get_tile(g, i, j) == 0 && get_tile(g, i + a, j + b))){
      return true;
    }
    i += a;
    j += b;
  }
  return false;
}

static void tri_insertion(float tab[], int taille){
   for(int i = 1; i < taille; i++){
       float elem = tab[i];
       int j;
       for(j = i; j > 0 && tab[j - 1] > elem; j--)
           tab[j] = tab[j - 1];
       tab[j] = elem;
   }
}
/*
static void incrementation(int *i1, int *i2, int incrementationI1, int incrementationI2){
  *i1 += incrementationI1;
  *i2 += incrementationI2;
}


static bool possible_lc(grid g, int x,int y, int addx, int addy){
  int tmpx = x;
  int tmpy = y;
  while(x < GRID_SIDE - addx && x >= 0 - addx && y < GRID_SIDE - addy && y >= 0 - addy){

      // Si deux cases collées sont indentiques alors return true
      if((get_tile(g, x, y) == get_tile(g, x + addx, y + addy) && get_tile(g, x, y) != 0))
	return true;

      // Si une case vide est suivie d'une case non vide alors return true
      else if(get_tile(g, x, y) == 0 && get_tile(g, x + addx, y + addy) != 0)
	return true;
      incrementation(&x, &y, addx, addy);
    }

    // Réinitialiser i ou j et incrémenter l'autre indice pour changer de ligne ou de colonne selon la direction
    if(addx == -1 || addx == 1)
      incrementation(&y, &x, 1, tmpx - x); 
    else
      incrementation(&x, &y, 1, tmpy - y); 
  return false;
}


static unsigned long int poids_fusion_1 (grid g, dir d){
  grid grid_copy = new_grid();
  copy_grid(g, grid_copy);
  do_move(grid_copy, d);
  return grid_score(grid_copy) - grid_score(g);
}

static dir strategy_jimmy_1(grid g){
  int cpt_up = poids_fusion(g, UP);
  int cpt_down = poids_fusion(g, DOWN);
  int cpt_left = poids_fusion(g, LEFT);
  int cpt_right = poids_fusion(g, RIGHT);
 
  if(can_move(g, UP) && can_move(g, LEFT)){
    if(cpt_up >= cpt_left)
      return UP;
    else
      return LEFT;
  }else{
    if(can_move(g, UP) || can_move(g, LEFT)){
      if(can_move(g, UP))
	return UP;
      else
	return LEFT;
    }else{
      if(!possible_lc(g, 0, 0, 1, 0) || !possible_lc(g, 0, 0, 0, 1)){
	if(can_move(g, RIGHT) && !possible_lc(g, 0, 0, 1, 0))
	  return RIGHT;
	else if(can_move(g, DOWN) && !possible_lc(g, 0, 0, 1, 0))
	  return DOWN;
	else{
	  if(cpt_right >= cpt_down)
	    return RIGHT;
	  else
	    return DOWN;
	}
      }else{
	if(cpt_right >= cpt_down)
	  return RIGHT;
	else
	  return DOWN;
      }
    }
  }
}
*/
