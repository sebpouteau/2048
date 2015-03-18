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

stractegy initStructure(){
  strategy strat = malloc(sizeof(*strat));
  strat->mem = NULL;
  strat->name = "Jimmy";
  strat->play_move = strategy_jimmy_1;
  strat->free_strategy = free_memless_strat;
}



/* ====================
   === STRATEGIE IA ===
   ==================== */


static unsigned long int poids_fusion(grid g, dir d);

dir strategy_jimmy_1(grid g){
  int cpt_up = poids_fusion(g,UP);
  int cpt_down = poids_fusion(g,DOWN);
  int cpt_left = poids_fusion(g,LEFT);
  int cpt_right = poids_fusion(g,RIGHT);
  
  if(can_move(g,UP) && can_move(g,LEFT)){
    if(cpt_up >= cpt_left)
      return UP;
    else
      return LEFT;
  }else{
    if(can_move(g,UP) || can_move(g,LEFT)){
      if(can_move(g,UP))
	return UP;
      else
	return LEFT;
    }else{
      if(!possible_lc(g,0,0,1,0) || !possible_lc(g,0,0,0,1)){
	if(can_move(g,RIGHT) && !possible_lc(g,0,0,1,0))
	  return RIGHT;
	else if(can_move(g,DOWN) && !possible_lc(g,0,0,1,0))
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


static unsigned long int poids_fusion(grid g, dir d){
  grid grid_copy = new_grid();
  copy_grid(g,grid_copy);
  do_move(grid_copy,d);
  return grid_score(grid_copy)-grid_score(g);
}


/* =============================
   === AFFICHE LES RESULTATS ===
   ============================= */

static long maximum_tile(grid g);

int main(int argc,char **argv){

  int n = 100;
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
 
  while(n>0){
    grid g= new_grid();
    add_tile(g);
    add_tile(g);

    int cpt_up;
    int cpt_down;
    int cpt_left;
    int cpt_right;
    
    while(!game_over(g)){
      strategy_jimmy_1(g)
    }
    if(maximum_tile(g) == 16)
      cpt_16 += 1;    if(maximum_tile(g) == 32)
      cpt_32 += 1;
    if(maximum_tile(g) == 64)
      cpt_64 += 1;
    if(maximum_tile(g) == 128)
      cpt_128 += 1;
    if(maximum_tile(g) == 256)
      cpt_256 += 1;
    if(maximum_tile(g) == 512)
      cpt_512 += 1;
    if(maximum_tile(g) == 1024)
      cpt_1024 += 1;
    if(maximum_tile(g) == 2048)
      cpt_1024 += 1;
    delete_grid(g);
    n -=1;
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


static long maximum_tile(grid g){
  long max_tile = 2;
  for(int i = 0; i<GRID_SIDE; i++){
    for(int j = 0; j<GRID_SIDE; j++){
      if(get_tile(g, i, j)>max_tile)
	max_tile = get_tile(g, i, j);
    }
  }
  return max_tile;
}


