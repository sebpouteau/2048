#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "../Projet-Jeu/src/grid.h"
#include <time.h>

//struct tabBestM{
//unsigned long int score;
//dir best; 
//};

#define MONO_WEIGHT 3
#define MONO_POW 4
#define SMOOTH_WEIGHT 7
#define EMPTY_WEIGHT 4


//static void display_grid(grid g);
static long maximum_tile(grid g);
//static tabBestM new_tabBestM();
static dir best_move(grid g,int depth,dir best);
static int empty_tiles(grid g);
static int monotonicity(grid g);
static int smoothness(grid g);
static bool max_in_corner(grid g);
//static grid best_grid(grid g)

int main(int argc,char **argv){
  srand(time(NULL));
  for (int j=0; j< 10;j++){
    grid g= new_grid();
    add_tile(g);
    add_tile(g);
    while(!game_over(g)){
      //play(g,best_move(g,UP,0,0,0));
    }
      
    printf("Tile max = %ld \n", maximum_tile(g));
    //delete_grid(g);
  
  }
}


/* static void display_grid(grid g){ */
/*   for (int i=0;i<GRID_SIDE;i++){ */
/*     for (int j=0;j<GRID_SIDE;j++){ */
/*       printf("%d ",get_tile(g,i,j)); */
/*     } */
/*     printf("\n"); */
/*   } */
/*   printf("\n"); */
/* } */


static long maximum_tile(grid g){
  long max_tile = 2;
  for(int i = 0; i<GRID_SIDE; i++){
    for(int j = 0; j< GRID_SIDE; j++){
      if(get_tile(g, i, j)>max_tile)
	max_tile = get_tile(g, i, j);
    }
  }
  return max_tile;
}




//static tabBestM new_tabBestM(){
//tabBestM tab;
  //assert(tab);
  //tab.score = 0;
  //tab.best = 0;
  //return tab;
//}



static dir best_move(grid g,int depth,dir best){
  if(depth == 4)
    return best;
  grid test = new_grid();
  copy_grid(g,test);
  for(dir i=UP;i<=RIGHT;i++){
    do_move(test,i);
  }
}

float eval_branch(grid g){
  float mono;
  if(max_in_corner)
    mono = pow(monotonicity(g)*MONO_WEIGHT,MONO_POW);
  else
    mono = monotonicity(g)*MONO_WEIGHT;
  float smooth = smoothness(g)*SMOOTH_WEIGHT;
  float empty = empty_tiles(g)*EMPTY_WEIGHT;
  return mono + smooth + empty + grid_score(g);
}


static int empty_tiles(grid g){
  int nb = 0;
  for(int i = 0; i<4;i++){
    for(int j = 0;j<4;j++){
      if(get_tile(g,i,j) == 0)
	nb += 1;
    }
  }
  return nb;
}



static int monotonicity(grid g){
  int variation_sign;
  int compteur = 0;
  if(get_tile(g,0,0)>get_tile(g,0,1))
    variation_sign = 1;
  else
    variation_sign = 0;
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(get_tile(g, i, j) < get_tile(g, i, j+1) && variation_sign == 1){
	variation_sign = 0;
	compteur += 1;
      }
      if(get_tile(g, i, j) > get_tile(g, i, j+1) && variation_sign == 0){
	variation_sign = 1;
	compteur += 1;
      }
      if(get_tile(g, j, i) < get_tile(g, j+1, i) && variation_sign == 1){
	variation_sign = 0;
	compteur += 1;
      }
      if(get_tile(g, j, i) > get_tile(g, j+1, i) && variation_sign == 0){
	variation_sign = 1;
	compteur += 1;
      }
    }
  }  
  return compteur*(-1);
}

static int smoothness(grid g){
  int sum = 0;
  for(int i = 0;i<3;i++){
    for(int j = 0;j<3;j++){
      if(get_tile(g,i,j)>=get_tile(g,i,j))
	sum+=(int)get_tile(g,i,j)-(int)(get_tile(g,i,j+1));
      else
	sum+=(int)get_tile(g,i,j+1)-(int)(get_tile(g,i,j));
      if(get_tile(g,j,i)>=get_tile(g,j+1,i))
	sum+=(int)get_tile(g,j,i)-(int)(get_tile(g,j+1,i));
      else
	sum+=(int)get_tile(g,j+1,i)-(int)(get_tile(g,j,i));
    }
  }
  return sum;
}

bool max_in_corner(grid g){
  tile max_tile = maximum_tile(g);
  return get_tile(g, 0, 0) == max_tile || get_tile(g, 3, 0) == max_tile || get_tile(g, 0, 3) == max_tile || get_tile(g, 3, 3) == max_tile;
}

/* grid best_grid(grid g){ */
/*   grid test = new_grid(); */
/*   copy_grid(g,test); */
/*   for(dir i = UP;i<=RIGHT;i++) */
/*     return g; */
/* } */
