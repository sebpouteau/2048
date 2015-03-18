#include "strategy.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
static bool objectif_atteint(grid g);
static long maximum_tile(grid g);


void free_memless_strat (strategy strat)
{
  free (strat);
}

  /* if (can_move(g,UP)){return UP;} */
  /* else if (can_move(g,LEFT)){return LEFT;} */
  /* else if (can_move(g,DOWN)){return DOWN;} */
  /* else if (can_move(g,RIGHT)){return RIGHT;} */

dir strategy_seb(strategy str, grid g){
  dir direction;
  

  return direction;
}

long int note_grid (grid g){
  long int cpt = 0;
  int cpt_case_empty = 0;
  for(int y = 0; y < GRID_SIDE; y++){
    for(int x = 0; x < GRID_SIDE; x++){
      cpt += get_tile(g,x,y);
      if(get_tile(g,x,y) == 0)
	cpt_case_empty++;
    }
  }
  int max = (int)maximum_tile(g);
  if( get_tile(g, 0, 0) == max || get_tile(g, GRID_SIDE -1 , 0) == max || get_tile(g, 0, GRID_SIDE-1) == max || get_tile(g, GRID_SIDE - 1, GRID_SIDE - 1) == max )
    cpt += 10;
  return cpt;
}
  

strategy init_Structure (){
  strategy str = malloc (sizeof(strategy));
  str->name = "strategy_seb";
  str->mem = NULL;
  str->free_strategy = free_memless_strat;
  str->play_move = strategy_seb;
  return str;
}


int main(int argc,char **argv){


  int n = 10;
  srand(time(NULL));
 
  while(n >0){
    grid g= new_grid();
    add_tile(g);
    add_tile(g);
  //int ch=0;
    strategy seb = init_Structure();
    dir direction;
    while(!game_over(g)){
      direction = strategy_seb(seb,g);
      play(g,direction);
    }
    printf("Objectif atteint? %s \n", objectif_atteint(g)?"oui":"non");
    printf("Tile max = %ld \n",(long int)pow(2, maximum_tile(g)));
    delete_grid(g);
    n -=1;
  }
}



static bool objectif_atteint(grid g){
  for(int i = 0; i<GRID_SIDE; i++){
    for(int j = 0; j< GRID_SIDE; j++){
      if(get_tile(g, i, j)>=2048)
	return true;
    }
  }
  return false;
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
