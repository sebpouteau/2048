#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "grid.h"

static bool egalite_grid(grid g, grid g1);

int main(){
  grid g = new_grid();
  //test-set_tile/get_tile;
  for(int i = 0;i<4;i++)
    set_tile(g,i,0,2);
  for(int i = 0;i<4;i++){
    if(get_tile(g,i,i)!=2){
      printf("Le set est False");
      break;
    }
    else{
      printf("Le set est Ok");
      break;
    }
  }
  printf("\n");
  //test copy_grid;
  grid g1 = new_grid();
  copy_grid(g,g1);
  printf( egalite_grid(g,g1) ? "Copy : Ok" : "Copy : False");
  printf("\n");
  
  // test get_grid_score/set_grid_score;
  do_move(g,UP);
  if(grid_score(g) != 8)
    printf("Le score est False");
  printf("Le score est Ok");
  printf("\n");

  //test Game_Over;
  int compt = 1;
  for(int i = 0;i<4;i++){
    for(int j = 0;j<4;j++){
      set_tile(g,i,j,compt);
      compt++;
    }
  }
  printf(game_over(g) ? "Game_Over : Ok" : "Game_Over : False");
  printf("\n");

  //test delete_grid;
  delete_grid(g);
  delete_grid(g1);
  if(g == NULL || g1 == NULL)
    printf("Delete_Grid : False");
  printf("Delete_Grid : Ok");
  printf("\n");
    
}

static bool egalite_grid(grid g,grid g1){
  bool b = true;
  for(int i = 0; i<4; i++){
    for(int j = 0; j<4;j++){
      if(get_tile(g,i,j)!= get_tile(g1,i,j))
	 b = false;
    }
  }
  return b;

  return EXIT_SUCCESS;
}
