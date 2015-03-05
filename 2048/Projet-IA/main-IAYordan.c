#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "../Projet-Jeu/src/grid.h"
#include "../Projet-Jeu/src/gridComplementaire.h"
#include <time.h>

struct tabBestM{
  unsigned long int score;
  dir best; 
};


static void display_grid(grid g);
static long maximum_tile(grid g);
static tabBestM new_tabBestM();
static dir best_move(grid g);
static void boucle_best_move(tabBestM tab,dir def, grid g, grid test);
static dir mouvement_impossible(grid g);

int main(int argc,char **argv){
  int n = 10;
  srand(time(NULL));
  while(n >0){
    grid g= new_grid();
    add_tile(g);
    add_tile(g);
    //int ch=0;
    grid test = new_grid();
     tabBestM tab = new_tabBestM();
     while(!game_over(g)){
       tab.score = grid_score(test);
       for(dir i = UP; i<=RIGHT;i++){
	 copy_grid(g,test);
	 play(test,i);
	 play(test,best_move(test));
	 if(tab.score < grid_score(test)){
	   tab.score = grid_score(test);
	   tab.best = i;
	 }
	 dir b = tab.best;
	 play(g,b);
       }
       printf("Tile max = %ld \n", maximum_tile(g));
       delete_grid(g);
       n-=1;
     }
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




static tabBestM new_tabBestM(){
  tabBestM tab;
  //assert(tab);
  tab.score = 0;
  tab.best = UP;
  return tab;
}

/* static void delete_tabBestM(tabBestM tab){ */
/*   free(tab); */
/* } */
  

static dir best_move(grid g){
  grid test = new_grid();
  tabBestM tab = new_tabBestM();
  boucle_best_move(tab,mouvement_impossible(test),g,test);

  
  dir b = tab.best;
  //delete_tabBestM(tab);
  delete_grid(test);
  return b;
}

static void boucle_best_move(tabBestM tab,dir def, grid g, grid test){
  for(dir i = UP;i<=RIGHT;i++){
    copy_grid(g,test);
    if(i == def)
      continue;
    do_move(test,i);
    if(grid_score(test) > tab.score){
      tab.score = grid_score(test);
      tab.best = i;
    }
  }
}    

static dir mouvement_impossible(grid g){
  for(dir i = UP; i<=RIGHT;i++){
    if(!can_move(g,i))
      return i;
  }
  return 0;
}
 
   
  
  
