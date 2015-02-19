#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "grid.h"

static bool egalite_grid(grid g, grid g1);

int main(){
  grid g = new_grid();
  bool valide=true;
  //test-set_tile/get_tile;
  for(int i = 0;i<4;i++)
    set_tile(g,i,0,2);
  for(int i = 0;i<4;i++){
    if(get_tile(g,i,i)!=2){
      valide=false;
      break;
    }
  }
  printf(valide?"set_tile :  Ok":"set_tile : False");
  printf("\n");
  
  
  //test copy_grid;
  grid g1 = new_grid();
  copy_grid(g,g1);
  printf( egalite_grid(g,g1) ? "Copy : Ok" : "Copy : False");
  printf("\n");
  
  valide = true;
  // test get_grid_score/set_grid_score;
  do_move(g,UP);
  if(grid_score(g) != 8)
    valide=false;
  printf(valide?"grid_score : Ok":"grid_score : False");
  printf("\n");
  
  valide = true;
  //test Game_Over;
  int compt = 1;
  for(int i = 0;i<GRID_SIDE;i++){
    for(int j = 0;j<GRID_SIDE;j++){
      set_tile(g,i,j,compt);
      compt++;
    }
  }
  if(!game_over(g)) // si il indique que ce n'est pas gameover c'est que ne fonctionne pas
    valide=false;
  set_tile (g,0,1,1);
  if (game_over(g))  //si il indique que c'est game_over c'est qu'il ne fonctionne pas
    valide=false;
  printf(valide ? "game_over : Ok" : "Game_Over : False");
  printf("\n");
  
  valide = true;
  for(int i = 0;i<GRID_SIDE;i++)
    for(int j = 0;j<GRID_SIDE;j++)
      set_tile(g,i,j,2);
  do_move(g,UP);
  do_move(g,DOWN);
  do_move(g,LEFT);
  do_move(g,RIGHT);
  if (get_tile(g,3,3)!=32)
    valide=false;
  set_tile(g,3,0,32);
  do_move(g,LEFT);
  if (get_tile(g,3,0)!=64)
    valide=false;
  set_tile(g,3,2,64);
  do_move(g,LEFT);
  if (get_tile(g,3,0)!=128)
    valide=false;
  printf(valide ?"do_move : Ok":"do_move : False");
  printf("\n");
  
  //test delete_grid;
  delete_grid(g);
  delete_grid(g1);
  if(g == NULL || g1 == NULL)
    valide =false;
  printf(valide ?"Delete_Grid : Ok":"Delete_Grid : False");
  printf("\n");

  valide = true;

  return EXIT_SUCCESS;  

}


static bool egalite_grid(grid g,grid g1){
  bool b = true;
  for(int i = 0; i<4; i++)
    for(int j = 0; j<4;j++)
      if(get_tile(g,i,j)!= get_tile(g1,i,j))
	 b = false;
  return b;
}
