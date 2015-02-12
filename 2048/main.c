#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "grid.h"
static void display_grid(grid g);

int main(int argc,char **argv){
  grid g = new_grid();
  for (int i=0;i<GRID_SIDE;i++){
    for (int j=0;j<GRID_SIDE;j++){
      set_tile(g,i,j,2);
    }
  }
  display_grid(g);
  do_move(g,UP);
  display_grid(g);
  do_move(g,DOWN);
  display_grid(g);
  do_move(g,LEFT);
  display_grid(g);
  do_move(g,RIGHT);
  display_grid(g);
  

}

static void display_grid(grid g){
  for (int i=0;i<GRID_SIDE;i++){
    for (int j=0;j<GRID_SIDE;j++){
      printf("%d ",get_tile(g,i,j));
    }
    printf("\n");
  }
  printf("\n");
}
