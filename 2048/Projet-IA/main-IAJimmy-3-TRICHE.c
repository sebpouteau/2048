#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "../Projet-Jeu/grid.h"
#include "../Projet-Jeu/gridIA.h"
#include <ncurses.h>
#include <curses.h>
#include <time.h>

//static void display_grid(grid g);
//static void display_gameOver(bool *continuer,int *reponse_valide);
//static bool objectif_atteint(grid g);
static long maximum_tile(grid g);
static grid strategie(grid g, int n);

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
    grid g = new_grid();
    
    while(!game_over(g)){
      g = strategie(g,10);
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


static grid strategie(grid g, int n){
  grid copy_g = new_grid();
  copy_grid(g, copy_g);
  grid copy2_g = new_grid();
  copy_grid(g, copy2_g);

  int premier = n;

  int cpt_up_copy_g;
  int cpt_down_copy_g;
  int cpt_left_copy_g;
  int cpt_right_copy_g;

  int cpt_up_copy2_g;
  int cpt_down_copy2_g;
  int cpt_left_copy2_g;
  int cpt_right_copy2_g;
  
  while(n>0){
    cpt_up_copy_g = poids_fusion(copy_g,UP);
    cpt_down_copy_g = poids_fusion(copy_g,DOWN);
    cpt_left_copy_g = poids_fusion(copy_g,LEFT);
    cpt_right_copy_g = poids_fusion(copy_g,RIGHT);

    if(can_move(copy_g,UP) && can_move(copy_g,LEFT)){
      if(premier == n)
	play(copy_g,UP);
      else{
	if(cpt_up_copy_g >= cpt_left_copy_g)
	  play(copy_g,UP);
	else
	  play(copy_g,LEFT);
      }
    }else{
      if(can_move(copy_g,UP) || can_move(copy_g,LEFT)){
	if(can_move(copy_g,UP))
	  play(copy_g,UP);
	else
	  play(copy_g,LEFT);
      }else{
	if(!possible_lc(copy_g,0,0,1,0) || !possible_lc(copy_g,0,0,0,1)){
	  if(can_move(copy_g,RIGHT) && !possible_lc(copy_g,0,0,1,0))
	    play(copy_g,RIGHT);
	  else if(can_move(copy_g,DOWN) && !possible_lc(copy_g,0,0,1,0))
	    play(copy_g,DOWN);
	  else{
	    if(premier == n)
	      play(copy_g,RIGHT);
	    else{
	      if(cpt_right_copy_g >= cpt_down_copy_g)
		play(copy_g,RIGHT);
	      else
		play(copy_g,DOWN);
	    }
	  }
	}else{
	  if(premier == n)
	    play(copy_g,RIGHT);
	  else{
	    if(cpt_right_copy_g >= cpt_down_copy_g)
	      play(copy_g,RIGHT);
	    else
	      play(copy_g,DOWN);
	  }
	}
      }
    }
        
    cpt_up_copy2_g = poids_fusion(copy2_g,UP);
    cpt_down_copy2_g = poids_fusion(copy2_g,DOWN);
    cpt_left_copy2_g = poids_fusion(copy2_g,LEFT);
    cpt_right_copy2_g = poids_fusion(copy2_g,RIGHT);
    
    if(can_move(copy2_g,UP) && can_move(copy2_g,LEFT)){
      if(premier == n)
	play(copy2_g,LEFT);
      else{
	if(cpt_up_copy2_g >= cpt_left_copy2_g)
	  play(copy2_g,UP);
	else
	  play(copy2_g,LEFT);
      }
    }else{
      if(can_move(copy2_g,UP) || can_move(copy2_g,LEFT)){
	if(can_move(copy2_g,UP))
	  play(copy2_g,UP);
	else
	  play(copy2_g,LEFT);
	premier--;
      }else{
	if(!possible_lc(copy2_g,0,0,1,0) || !possible_lc(copy2_g,0,0,0,1)){
	  if(can_move(copy2_g,RIGHT) && !possible_lc(copy2_g,0,0,1,0)){
	    play(copy2_g,RIGHT);
	    premier--;
	  }
	  else if(can_move(copy2_g,DOWN) && !possible_lc(copy2_g,0,0,1,0)){
	    play(copy2_g,DOWN);
	    premier--;
	  }
	  else{
	    if(premier == n)
	      play(copy2_g,DOWN);
	    else{
	      if(cpt_right_copy2_g >= cpt_down_copy2_g)
		play(copy2_g,RIGHT);
	      else
		play(copy2_g,DOWN);
	    }
	  }
	}else{
	  if(premier == n)
	    play(copy2_g,DOWN);
	  else{
	    if(cpt_right_copy2_g >= cpt_down_copy2_g)
	      play(copy2_g,RIGHT);
	    else
	      play(copy2_g,DOWN);
	  }
	}
      }
    }
    n--;
  }
  if(grid_score(copy_g)>=grid_score(copy2_g))
    return copy_g;
  else
    return copy2_g;
}


/*
    cpt_up = poids_fusion(copy_g,0,0,1,0);
    cpt_down = poids_fusion(copy_g,3,0,-1,0);
    cpt_left = poids_fusion(copy_g,0,0,0,1);
    cpt_right = poids_fusion(copy_g,0,3,0,-1);
    if(can_move(copy_g,UP) && can_move(copy_g,LEFT)){
      if(cpt_up >= cpt_left)
	play(copy_g,UP);
      else
	play(copy_g,LEFT);
    }else{
      if(can_move(copy_g,UP) || can_move(copy_g,LEFT)){
	if(can_move(copy_g,UP))
	  play(copy_g,UP);
	else
	  play(copy_g,LEFT);
      }else{
	if(!possible_ligne(copy_g,0,0,1,0) || !possible_ligne(copy_g,0,0,0,1)){
	  if(can_move(copy_g,RIGHT) && !possible_ligne(copy_g,0,0,1,0))
	    play(copy_g,RIGHT);
	  else if(can_move(copy_g,DOWN) && !possible_ligne(copy_g,0,0,1,0))
	    play(copy_g,DOWN);
	  else{
	    if(cpt_right >= cpt_down)
	      play(copy_g,RIGHT);
	    else
	      play(copy_g,DOWN);
	  }
	}else{
	  if(cpt_right >= cpt_down)
	    play(copy_g,RIGHT);
	  else
	    play(copy_g,DOWN);
	}
      }
    }
*/
