#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "../src/grid.h"


int main(int argc,char **argv){
  grid g = new_grid();
  for (int i=0;i<GRID_SIDE;i++){
    for (int j=0;j<GRID_SIDE;j++){
      set_tile(g,i,j,2);
    }
  }
  int i=0;
  while(i<100000){
  do_move(g,UP);
  do_move(g,DOWN);
  do_move(g,LEFT);
  do_move(g,RIGHT);
  i++;
  }
}

