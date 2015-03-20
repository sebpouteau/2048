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

static float poids_fusion (grid g, dir d, int n);
static bool possible_lc (grid g, int i, int j, int a,int b);
static void tri_insertion(float tab[], dir tab_dir[], int taille);
static long maximum_tile (grid g);

static dir strategy_jimmy (grid g){
  float cpt_up = poids_fusion(g, UP, 2);
  float cpt_down = poids_fusion(g, DOWN, 2);
  float cpt_left = poids_fusion(g, LEFT, 2);
  float cpt_right = poids_fusion(g, RIGHT, 2);
  
  float tab[4] = {cpt_up, cpt_down, cpt_left, cpt_right};
  dir tab_dir[4] = {UP, DOWN, LEFT, RIGHT};
  tri_insertion(tab, tab_dir, 4);

  if(can_move(g, UP)){
    if(can_move(g, LEFT)){
      if(can_move(g, RIGHT) && !possible_lc(g, 0, 0, 1, 0)){
	if(can_move(g, DOWN) && !possible_lc(g, 0, 0, 0, 1))
	  return tab_dir[3];
	else{ // Si !can_move(g, DOWN) 
	  if(tab_dir[3] == DOWN)
	    return tab_dir[2];
	  return tab_dir[3];
	}
      }else{ //si !can_move(g, RIGHT)
	if(can_move(g, DOWN) && !possible_lc(g, 0, 0, 0, 1)){
	  if(tab_dir[3] == RIGHT)
	    return tab_dir[2];
	  return tab_dir[3];
	}else{ // si !can_move(g, RIGHT) && !can_move(g, DOWN)
	  if(tab_dir[3] == DOWN || tab_dir[3] == RIGHT){
	    if(tab_dir[2] == DOWN || tab_dir[2] == RIGHT)
	      return tab_dir[1];
	    return tab_dir[2];
	  }
	  return tab_dir[3];
	}
      }
    }else{ // si !can_move(g, LEFT);
      if(can_move(g, RIGHT) && !possible_lc(g, 0, 0, 1, 0)){
	if(can_move(g, DOWN) && !possible_lc(g, 0, 0, 0, 1)){
	  if(tab_dir[3] == LEFT)
	    return tab_dir[2];
	  return tab_dir[3];
	}else{ // Si !can_move(g, DOWN) && !can_move(g, LEFT)
	  if(tab_dir[3] == DOWN || tab_dir[3] == LEFT){
	    if(tab_dir[2] == DOWN || tab_dir[2] == LEFT)
	      return tab_dir[1];
	    return tab_dir[2];
	  }
	  return tab_dir[3];
	}
      }else{ //si !can_move(g, RIGHT) && !can_move(g, LEFT)
	if(can_move(g, DOWN) && !possible_lc(g, 0, 0, 1, 0)){
	  if(tab_dir[3] == RIGHT || tab_dir[3] == LEFT){
	    if(tab_dir[2] == RIGHT || tab_dir[2] == LEFT)
	      return tab_dir[1];
	    return tab_dir[2];
	  }
	  return tab_dir[3];
	}else // si !can_move(g, RIGHT) && !can_move(g, DOWN) && !can_move(g, LEFT)
	  return UP;
      }
    }
  }else{ // si !can-move(g, UP)
    if(can_move(g, LEFT)){
      if(can_move(g, RIGHT) && !possible_lc(g, 0, 0, 1, 0)){
	if(can_move(g, DOWN) && !possible_lc(g, 0, 0, 0, 1)){
	  if(tab_dir[3] == UP)
	    return tab_dir[2];
	  return tab_dir[3];
	}else{ // Si !can_move(g, DOWN) && !possible_lc(g, 0, 0, 1, 0) && !can_move(g, UP)
	  if(tab_dir[3] == DOWN || tab_dir[3] == UP){
	    if(tab_dir[2] == DOWN || tab_dir[2] == UP)
	      return tab_dir[1];
	    return tab_dir[2];
	  }
	  return tab_dir[3];
	}
      }else{ //si !can_move(g, RIGHT) && !possible_lc(g, 0, 0, 0, 1) && !can_move(g, UP)
	if(can_move(g, DOWN) && !possible_lc(g, 0, 0, 0, 1)){
	  if(tab_dir[3] == RIGHT || tab_dir[3] == UP){
	    if(tab_dir[2] == RIGHT || tab_dir[2] == UP)
	      return tab_dir[1];
	    return tab_dir[2];
	  }
	  return tab_dir[3];
	}else // si !can_move(g, RIGHT) && !can_move(g, DOWN) && !can_move(g, UP)
	  return LEFT;
      }
    }else{ // si !can_move(g, LEFT);
      if(can_move(g, RIGHT) && !possible_lc(g, 0, 0, 1, 0)){
	if(can_move(g, DOWN) && !possible_lc(g, 0, 0, 0, 1)){
	  if(tab_dir[3] == UP || tab_dir[3] == LEFT){
	    if(tab_dir[2] == UP || tab_dir[2] == LEFT)
	      return tab_dir[1];
	    return tab_dir[2];
	  }
	  return tab_dir[3];
	}else
	  return RIGHT;
      }
      else if(can_move(g, DOWN) && !possible_lc(g, 0, 0, 0, 1))
	return DOWN;
      else if(can_move(g, DOWN) && can_move(g, RIGHT)){
	if(cpt_right >= cpt_down)
	  return RIGHT;
	return DOWN;
      }
      else if(can_move(g, RIGHT))
	return RIGHT;
      return DOWN;
    }
  }
}


static float poids_fusion (grid g, dir d, int n){  
  float poids = 0;
  grid g_copy = new_grid();
  copy_grid(g, g_copy);
  do_move(g_copy, d);
  poids = grid_score(g_copy) - grid_score(g);

  while(n > 0){
    n--;
    float poids_tmp = 0;
    int tmp = 0;
    if(can_move(g_copy, UP)){
      tmp += 1;
      poids_tmp += poids_fusion(g_copy, UP, n);
    }
    if(can_move(g_copy, LEFT)){
      tmp += 1;
      poids_tmp += poids_fusion(g_copy, LEFT, n);
    }
    if(can_move(g_copy, RIGHT)){
      tmp += 1;
      poids_tmp += poids_fusion(g_copy, RIGHT, n);
    }
    if(can_move(g_copy, DOWN)){
      tmp += 1;
      poids_tmp += poids_fusion(g_copy, DOWN, n);
    }
    if(tmp != 0)
      poids += (poids_tmp / (tmp)) / (n + 1);
  }
  
  if(maximum_tile(g) == get_tile(g_copy, 0, 0))
    poids += get_tile(g_copy, 0, 0) * 2;
  if(maximum_tile(g) == get_tile(g_copy, 1, 0))
    poids += get_tile(g_copy, 1, 0);
  if(maximum_tile(g) == get_tile(g_copy, 0, 1))
    poids += get_tile(g_copy, 0, 1);

  // -----------
  // On compte le nombre de case vide
  int nb_case_empty = 0;
  for(int i = 0; i < GRID_SIDE; i++){
    for(int j = 0; j < GRID_SIDE; j++){
      if(get_tile(g_copy, i, j) == 0)
	nb_case_empty += 1;
    }
  }
  poids += nb_case_empty;
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
        float tmp = (40 - nb_case_empty*2);
	while(cpti + 1 < GRID_SIDE){
	  cpti += 1;
	  if(get_tile(g_copy, i, j) == get_tile(g_copy, cpti, j))
	    poids += get_tile(g_copy, i, j) / tmp;
	  else if(get_tile(g_copy, cpti, j) == 0){
	    if(get_tile(g_copy, i, j) == 2)
	      poids += (get_tile(g_copy, i, j) * 9 / 10) / nb_case_empty;	    
	    if(get_tile(g_copy, i, j) == 4)
	      poids += (get_tile(g_copy, i, j) / 10) / nb_case_empty;
	  }
	  else
	    break;
	}

        while(cpti - 1 >= 0){
	  cpti -= 1;
	  if(get_tile(g_copy, i, j) == get_tile(g_copy, cpti, j))
	    poids += get_tile(g_copy, i, j) / tmp;
	  else if(get_tile(g_copy, cpti, j) == 0){
	    if(get_tile(g_copy, i, j) == 2)
	      poids += (get_tile(g_copy, i, j) * 9 / 10) / nb_case_empty;
	    if(get_tile(g_copy, i, j) == 4)
	      poids += (get_tile(g_copy, i, j) / 10) / nb_case_empty;
	  }
	  else
	    break;
	}
	
	while(cptj + 1 < GRID_SIDE){
	  cptj += 1;
	  if(get_tile(g_copy, i, j) == get_tile(g_copy, i, cptj))
	    poids += get_tile(g_copy, i, j) / tmp;
	  else if(get_tile(g_copy, cpti, j) == 0){
	    if(get_tile(g_copy, i, j) == 2)
	      poids += (get_tile(g_copy, i, j) * 9 / 10) / nb_case_empty;
	    if(get_tile(g_copy, i, j) == 4)
	      poids += (get_tile(g_copy, i, j) / 10) / nb_case_empty;
	  } 
	  else
	    break;
	}
	
        while(cptj - 1 >= 0){
	  cptj -= 1;
	  if(get_tile(g_copy, i, j) == get_tile(g_copy, i, cptj))
	    poids += get_tile(g_copy, i, j) / tmp;
	  else if(get_tile(g_copy, cpti, j) == 0){
	    if(get_tile(g_copy, i, j) == 2)
	      poids += (get_tile(g_copy, i, j) * 9 / 10) / nb_case_empty;
	    if(get_tile(g_copy, i, j) == 4)
	      poids += (get_tile(g_copy, i, j) / 10) / nb_case_empty;
	  }
	  else
	    break;
	}
      } // fin du premier if
    } // fin du for(int j = 0; ...)
  } // fin du for(int i = 0; ...)
  return poids;
}


/* ===========================
   == AFFICHE LES RESULTATS ==
   =========================== */

static long maximum_tile (grid g);

int main (int argc, char **argv){

  int n = 100; // Indice de décrémentation boucle while
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
      if(can_move(g, strategy_jimmy(g))){
	 play(g, strategy_jimmy(g));
      }
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
      cpt_2048 += 1;

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

static void tri_insertion(float tab[], dir tab_dir[], int taille){
  int j;
  for(int i = 1; i < taille; i++){
    float elem = tab[i];
    dir elem_dir = tab_dir[i];
    for(j = i; j > 0 && tab[j - 1] > elem; j--){
      tab[j] = tab[j - 1];
      tab_dir[j] = tab_dir[j - 1];
    }
    tab_dir[j] = elem_dir;
    tab[j] = elem;
  }
}


/*
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
