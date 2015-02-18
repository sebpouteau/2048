#include <stdlib.h>
#include <stdio.h>
#include "grid.h"
#include "gridIA.h"

int poids_fusion(grid g, dir d){
  grid grid_copie= new_grid();
  copy_grid(g,grid_copy);
  do_move(grid_copy,d);
  return score_grid(grid_copy)-score_grid(g);
}

int nombre_fusion(grid g,dir d){
  grid grid_copie= new_grid();
  copy_grid(g,grid_copy);
  do_move(grid_copy,d);
  return get_nbr_case_empty(g)-get_nbr_case_empty(grid_copy);
}

/* test si la ligne ou la colonne en question peut faire une fusion */
bool possible_lc(grid g, int i, int j, int a,int b){
  while(i<4-a && i>=0-a && j<4-b && j>=0-b){
    if ((get_tile(g,i,j)==get_tile(g,i+a,j+b)&&get_tile(g,i,j)!=0)||(get_tile(g,i,j)==0 && get_tile(g,i+a,j+b))){
      return true;
    }
    i+=a;
    j+=b;
  }
  return false;
}
