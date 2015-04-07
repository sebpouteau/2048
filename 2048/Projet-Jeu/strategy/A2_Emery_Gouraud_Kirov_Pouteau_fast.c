#include <strategy.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <grid.h>
#include <stdlib.h>

#define PROFONDEUR 2
#define CASE_UP 0
#define CASE_LEFT 1
#define CASE_DOWN 2
#define CASE_RIGHT 3
#define MOVE_IMPOSSIBLE -999999999 
#define ANY_CASE_EMPTY -9999999

strategy A2_Emery_Gouraud_Kirov_Pouteau_fast ();
static dir strategy_fast(strategy str, grid g);
static dir meilleur_direction(grid g);
static long int repetition_grid(grid g, int nombre, dir *d);
static long maximum_tile(grid g);
static long int maximum(long int l,long int l1,dir d, dir d1, dir *d2);
static long int note_grid (grid g);

strategy A2_Emery_Gouraud_Kirov_Pouteau_fast (){
  strategy str = malloc (sizeof(struct strategy_s));
  str->name = "strateg";
  str->mem = NULL;
  str->free_strategy = free_memless_strat;
  str->play_move = strategy_fast;
  return str;
}

void free_memless_strat (strategy strat){
  free (strat);
}

static dir meilleur_direction(grid g){
  int nombre = PROFONDEUR;
  dir d = UP;
  repetition_grid(g,nombre,&d);
  return d;
 }

static dir strategy_fast(strategy str, grid g){
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
  // Traite les 4 mouvements possibles et stock leur valeur dans un tableau
  for (int i = UP; i<=RIGHT ; i++){
    long int note = 0;
    int cpt = 0;
    indice_tab++;
    copy_grid(g,g_copy);

    if ( can_move(g_copy,i)){
      do_move(g_copy,i);
      // Génère les 2 et les 4 dans chaque possition vide
      for(int y = 0; y < GRID_SIDE; y++)
	for(int x = 0; x < GRID_SIDE; x++)
	  if(get_tile(g_copy,x,y) == 0){
	    set_tile(g_copy,x,y,1);
	    // Rappel la fonction et multiplie le resultat par 9  car 90% de chance d'avoir un 2
	    note +=(long int) 9*repetition_grid(g_copy,nombre-1, d);
	    set_tile(g_copy,x,y,2);
	    // Rappel la fonction et multiplie le resultat par 9  car 10% de chance d'avoir un 4
	    note +=(long int) repetition_grid(g_copy,nombre-1, d);
	    set_tile(g_copy,x,y,0);
	    cpt +=10;    
	  }
      // si pas de case vide alors ont met une mauvaise note
      if (cpt == 0)
	tab[indice_tab]= ANY_CASE_EMPTY;
      // renvoye 
      else
	tab[indice_tab]=(long int)note/cpt;
    }  
    // si le mouvement est impossible ont met une très mauvaise note
   else {
      tab[indice_tab] = MOVE_IMPOSSIBLE;
    }
  }    
  delete_grid(g_copy);
  dir mdU_L;
  dir mdD_R;
  //selctionne la meilleur valeur obtenu après un UP et un LEFT et stock la direction dans mdU_L
  long int max_UD = maximum(tab[CASE_UP],tab[CASE_LEFT],UP,LEFT,&mdU_L);
  //selectionne la meilleur valeur obtenu apres un DOWN et un RIGHT et la direction stock dans mdW_R
  long int max_LR = maximum(tab[CASE_DOWN],tab[CASE_RIGHT],DOWN,RIGHT,&mdD_R);
  // renvoye la meilleur valeur obtenur apres un mdU_L et un mdD_R et stock la direction dans d
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



/* ================================== 
                  NOTE
   ================================== */


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
