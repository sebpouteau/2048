#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "../Projet-Jeu/src/grid.h"
#include <time.h>

struct tabBestM{
float score;
dir best; 
};

#define MONO_WEIGHT 3
#define MONO_POW 4
#define SMOOTH_WEIGHT 7
#define EMPTY_WEIGHT 4
#define SCORE_WEIGHT 0.1


//static void display_grid(grid g);
static long maximum_tile(grid g);
static struct tabBestM new_tabBestM();
static dir best_move(grid g);
static int empty_tiles(grid g);
static int monotonicity(grid g);
static int smoothness(grid g);
static bool max_in_corner(grid g);
//static grid best_grid(grid g);
static void fill_tabG(grid g,grid* tab);
static float eval_tabG(grid g,grid* tab);


int main(int argc,char **argv){
  srand(time(NULL));
  for (int j=0; j< 10;j++){
    grid g= new_grid();
    add_tile(g);
    add_tile(g);
    while(!game_over(g)){
      play(g,best_move(g));
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




static struct tabBestM new_tabBestM(){
  struct tabBestM tab;
  //assert(tab);
  tab.score = 0;
  tab.best = 0;
  return tab;
}



static dir best_move(grid g){
  grid test = new_grid();
  struct tabBestM tab = new_tabBestM();
  for(dir i=UP;i<=RIGHT;i++){
    copy_grid(g,test);
    if(!can_move(test,i))
      continue;
    do_move(test,i);
    grid* tabG = malloc((2*empty_tiles(test))*sizeof(grid*));
    fill_tabG(test,tabG);
    float score_tabG = eval_tabG(test,tabG);
    if(tab.score < score_tabG){
      tab.score = score_tabG;
      tab.best = i;
    }
  }
  dir b = tab.best;
  //free(tab);
  delete_grid(test);
  return b;
}

/* static dir best_move2(grid g, int depth,){ */
/*   if (depth == 0) */
/*     return dir; */

/*   else{ */
/*     grid * t=generer_grilles(g,depth); */
/*     dir d = moyenne_mouvements(t); */
/*     return best_move2(g,depth--) */

/*   } */
/* } */

static void fill_tabG(grid g,grid* tab){
  grid copy = new_grid();
  for(int i = 0;i<empty_tiles(g);i++){
    copy_grid(g,copy);
    for(int j=0;j<4;j++)
      for(int k = 0;k<4;k++){
	if(get_tile(copy,j,k) == 0){
	  set_tile(copy,j,k,2);
	  tab[i]=copy;
	}
      }
    for(int j=0;j<4;j++)
      for(int k = 0;k<4;k++){
	if(get_tile(copy,j,k) == 0){
	  set_tile(copy,j,k,4);
	  tab[i]=copy;
	}
      }
  }
}



float eval_branch(grid g){
  float mono;
  if(max_in_corner(g))
    mono = log(monotonicity(g)+1/MONO_WEIGHT);
  else
    mono = monotonicity(g)*MONO_WEIGHT;
  float smooth = smoothness(g)*SMOOTH_WEIGHT;
  float empty = empty_tiles(g)*EMPTY_WEIGHT;
  float score = grid_score(g) * SCORE_WEIGHT;
  return mono + smooth + empty + score;
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

static float eval_tabG(grid g,grid* tab){
  float score = 0;
  int empty = empty_tiles(g);
  for(int i = 0; i < empty; i++)
    score += eval_branch(tab[i])*0.9;
  for(int i = empty; i < empty*2; i++)
    score += eval_branch(tab[i])*0.1;
  return score/(empty*10);
}  

/* grid best_grid(grid g){ */
/*   grid test = new_grid(); */
/*   copy_grid(g,test); */
/*   for(dir i = UP;i<=RIGHT;i++) */
/*     return g; */
/* } */
