#include <stdlib.h>
#include <stdio.h>
#include "grid.h"
#include "gridIA.h"

unsigned long int poids_fusion(grid g, dir d){
  grid grid_copy = new_grid();
  copy_grid(g,grid_copy);
  do_move(grid_copy,d);
  return grid_score(grid_copy)-grid_score(g);
}

unsigned long int poids_fusion_jimmy(grid g, dir d){
  grid grid_copy = new_grid();
  copy_grid(g,grid_copy);
  do_move(grid_copy,d);
  int tile_up_left = (int)(get_tile(g,0,0)/4);
  return grid_score(grid_copy)-grid_score(g)+tile_up_left;
}

 int nombre_fusion(grid g,dir d){
   return 0;
 }
/*   grid grid_copy= new_grid(); */
/*   copy_grid(g,grid_copy); */
/*   do_move(grid_copy,d); */
/*   return get_nbr_case_empty(g)-get_nbr_case_empty(grid_copy); */
/* } */

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

bool egalite_grid(grid g,grid g1){
  bool b = true;
  for(int i = 0; i<4; i++)
    for(int j = 0; j<4;j++)
      if(get_tile(g,i,j)!= get_tile(g1,i,j))
	 b = false;
  return b;
}
