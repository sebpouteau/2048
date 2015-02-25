#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "../src/grid.h"
#include "../src/gridIA.h"


int main(){
  grid g = new_grid();
  bool valide=true;
 
  //test-set_tile/get_tile;
  for(int i = 0;i<4;i++)
    set_tile(g,i,0,2);
  for(int i = 0;i<4;i++){
    if(get_tile(g,i,0)!=2){
      valide=false;
      break;
    }
  }
  printf(valide?"Test 1/6 - set_tile :  Ok":"Test 1/6 - set_tile : False");
  printf("\n");
  
  
  //test copy_grid;
  grid g1 = new_grid();
  copy_grid(g,g1);
  printf( egalite_grid(g,g1) ? "Test 2/6 - copy_grid : Ok" : "Test 2/6 - copy_grid : False");
  printf("\n");
  
  valide = true;
  // test get_grid_score/set_grid_score;
  do_move(g,UP);
  if(grid_score(g) != 8)
    valide=false;
  printf(valide?"Test 3/6 - grid_score : Ok":"Test 3/6 - grid_score : False");
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
  printf(valide ? "Test 4/6 - game_over : Ok" : "Test 4/6 - Game_Over : False");
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
  printf(valide ?"Test 5/6 - do_move : Ok":"Test 5/6 - do_move : False");
  printf("\n");
  
  //test delete_grid;
  delete_grid(g);
  delete_grid(g1);
  if(g == NULL || g1 == NULL)
    valide =false;
  printf(valide ?"Test 6/6 - delete_Grid : Ok":"Test 6/6 - delete_Grid : False");
  printf("\n");
  
  valide = true;
  
  return EXIT_SUCCESS;  
}



