#include <strategy.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <grid.h>
#include <stdlib.h>

#define DEPTH 2
#define CASE_UP 0
#define CASE_LEFT 1
#define CASE_DOWN 2
#define CASE_RIGHT 3
#define MOVE_IMPOSSIBLE -999999999 
#define ANY_CASE_EMPTY -9999999

/* define pour la note de la grille */
#define BONUS_CORNER 3000
#define BONUS_HIGH_MAXIMUM 600
#define BONUS_SCORE 60
#define BONUS_CASE_EMPTY 750
#define MALUS_SIGN_CHANGE 1025

strategy A2_Emery_Gouraud_Kirov_Pouteau_low();
static dir best_move(strategy str, grid g);
static long int repetition_grid(grid g, int depth, dir *d);
static long int maximum(long int l, long int l1, dir d, dir d1, dir *d2);
static long int grid_note(grid g);
static long int monotonicity(grid g, int x, int y);

strategy A2_Emery_Gouraud_Kirov_Pouteau_low(){
  strategy str = malloc(sizeof(struct strategy_s));
  str->name = "strategy_low";
  str->mem = NULL;
  str->free_strategy = free_memless_strat;
  str->play_move = best_move;
  return str;
}

void free_memless_strat(strategy strat){
  free(strat);
}

static dir best_move(strategy str, grid g){
  int depth = DEPTH;
  dir d = UP;
  int cpt_case_empty = 0;
  for(int y = 0; y < GRID_SIDE; y++){
    for(int x = 0; x < GRID_SIDE; x++){
      if(get_tile(g, x, y) == 0){
	cpt_case_empty++;
      }
    }
  }
  if (cpt_case_empty < 6)
    depth++;
  if (cpt_case_empty < 2)
    depth++;
  if (cpt_case_empty < 1)
    depth++;
  repetition_grid(g, depth, &d);
  return d;
}

static long int repetition_grid(grid g, int depth, dir *d){
  if(game_over(g)){
    return 0;
  }
  if(depth == 0){
    return grid_note(g);    
  }
  long int tab[4];
  grid g_copy = new_grid();
  int indice_tab = -1;
  // Traite les 4 mouvements possibles et stock leur valeur dans un tableau
  for(int i = UP; i <= RIGHT; i++){
    long int note = 0;
    int cpt = 0;
    indice_tab++;
    copy_grid(g, g_copy);
    if(can_move(g_copy, i)){
      do_move(g_copy, i);
      // Genere les 2 et les 4 dans chaque position vide
      for(int y = 0; y < GRID_SIDE; y++)
	for(int x = 0; x < GRID_SIDE; x++)
	  if(get_tile(g_copy, x, y) == 0){
	    set_tile(g_copy,x, y, 1);
	    // Rappel la fonction et multiplie le resultat par 9  car 90% de chance d'avoir un 2
	    note += (long int) 9 * repetition_grid(g_copy, depth - 1, d);
	    set_tile(g_copy, x, y, 2);
	    // Rappel la fonction et multiplie le resultat par 9  car 10% de chance d'avoir un 4
	    note += (long int) repetition_grid(g_copy, depth - 1, d);
	    set_tile(g_copy, x, y, 0);
	    cpt += 10;    
	  }
      // si pas de case vide alors ont met une mauvaise note
      if(cpt == 0)
	tab[indice_tab] = ANY_CASE_EMPTY;
      // renvoye 
      else
	tab[indice_tab] = (long int)(note / cpt);
    }  
    // si le mouvement est impossible ont met une mauvaise note
    else{
      tab[indice_tab] = MOVE_IMPOSSIBLE;
    }
  }    
  delete_grid(g_copy);
  dir mdU_L;
  dir mdD_R;
  //selctionne la meilleur valeur obtenu apres un UP et un LEFT et stock la direction dans mdU_L
  long int max_UD = maximum(tab[CASE_UP], tab[CASE_LEFT], UP, LEFT, &mdU_L);
  //selectionne la meilleur valeur obtenu apres un DOWN et un RIGHT et la direction stock dans mdW_R
  long int max_LR = maximum(tab[CASE_DOWN], tab[CASE_RIGHT], DOWN, RIGHT, &mdD_R);
  // renvoye la meilleur valeur obtenur apres un mdU_L et un mdD_R et stock la direction dans d
  return maximum(max_UD, max_LR, mdU_L, mdD_R, d);  
}

long int maximum(long int max0, long int max1, dir dir0, dir dir1, dir *d2){
  if(max0 >= max1){
    *d2 = dir0;
    return max0;
  }
  *d2 = dir1;
  return max1;
}


/* ================================== 
                 NOTE
   ================================== */

long int grid_note(grid g){
  long int cpt = 0;
  int cpt_case_empty = 0;
  int cpt_sign_change = 0;
  int max = 0;
  //Parcours de la grille
  for(int y = 0; y < GRID_SIDE; y++){
    for(int x = 0; x < GRID_SIDE; x++){
      cpt += get_tile(g, x, y);
      // Determination de la valeur maximum 
      if(get_tile(g, x, y) > max)
	max = get_tile(g, x, y);
      // Determination du nombre de case vide
      if(get_tile(g, x, y) == 0){
	cpt_case_empty++;
      }
      // Monotonicite de la grille 
      cpt_sign_change += monotonicity(g, x, y);
    }
  }
  // Bonus coin superieur gauche
  if(get_tile(g, 0, 0) == max)
    cpt += BONUS_CORNER * max;
  // Autres bonus
  cpt += BONUS_HIGH_MAXIMUM * pow(2, max);
  cpt += BONUS_SCORE * grid_score(g);
  cpt += BONUS_CASE_EMPTY * cpt_case_empty;
  // Malus si pas monotone
  cpt -= MALUS_SIGN_CHANGE * cpt_sign_change;

  return cpt;
}

long int monotonicity(grid g, int x, int y){
  int cpt_sign_change = 0;
  if(x < GRID_SIDE - 1){
    if(get_tile(g, x, y) < get_tile(g, x + 1, y))
      cpt_sign_change += get_tile(g, x + 1, y) - get_tile(g, x, y);
    if(get_tile(g, y, x) < get_tile(g, y, x + 1))
      cpt_sign_change += get_tile(g, y, x + 1) - get_tile(g, y, x);
  }  
  return cpt_sign_change;
}