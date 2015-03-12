#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "../Projet-Jeu/src/grid.h"
#include "gridIA.h"
#include <ncurses.h>
#include <curses.h>
#include <time.h>

//static void display_grid(grid g);
//static void display_gameOver(bool *continuer,int *reponse_valide);
//static bool objectif_atteint(grid g);
static long maximum_tile(grid g);

int main(int argc,char **argv){

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
 
  while(n>0){
    grid g= new_grid();
    add_tile(g);
    add_tile(g);
    int cpt = -1;
    int cpt_up;
    int cpt_down;
    int cpt_left;
    int cpt_right;
    
    while(!game_over(g)){
      cpt_up = poids_fusion_jimmy(g,UP);
      cpt_down = poids_fusion_jimmy(g,DOWN);
      cpt_left = poids_fusion_jimmy(g,LEFT);
      cpt_right = poids_fusion_jimmy(g,RIGHT);
      if(can_move(g,UP) && can_move(g,LEFT)){
	if(cpt != -1){
	  if(cpt == 0 && possible_lc(g,0,0,1,0)){
	    cpt = -1;
	    play(g,UP);
	  }
	  if(cpt == 1 && possible_lc(g,0,0,0,1)){
	    cpt = -1;
	    play(g,LEFT);
	  }
	  cpt = -1;
	}
	if(cpt_up >= cpt_left){
	  play(g,UP);
	}else{
	  play(g,LEFT);
	}
      }else{
	if(can_move(g,UP) || can_move(g,LEFT)){
	  if(can_move(g,UP))
	    play(g,UP);
	  else
	    play(g,LEFT);
	}else{
	  if(!possible_lc(g,0,0,1,0) || !possible_lc(g,0,0,0,1)){
	    if(can_move(g,RIGHT) && !possible_lc(g,0,0,1,0))
	      play(g,RIGHT);
	    else if(can_move(g,DOWN) && !possible_lc(g,0,0,1,0))
	      play(g,DOWN);
	    else{
	      if(cpt_right >= cpt_down){
		play(g,RIGHT);
	      }else{
		play(g,DOWN);
	      }
	    }
	  }else{
	    if(cpt_right >= cpt_down){
	      cpt = 1;
	      play(g,RIGHT);
	    }else{
	      cpt = 0;
	      play(g,DOWN);
	    }
	  }
	}
      }
    }
    //printf("Objectif atteint? %s \n", objectif_atteint(g)?"oui":"non");
    //printf("Tile max = %ld \n", maximum_tile(g));
    if(maximum_tile(g) == 16)
      cpt_16 += 1;
    if(maximum_tile(g) == 32)
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
  printf("\n ---------------- \n");
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

/*
static bool objectif_atteint(grid g){
  for(int i = 0; i<GRID_SIDE; i++){
    for(int j = 0; j<GRID_SIDE; j++){
      if(get_tile(g, i, j)>=2048)
	return true;
    }
  }
  return false;
}
*/

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

