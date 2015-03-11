#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "../Projet-Jeu/src/grid.h"
//#include "../Projet-Jeu/src/gridComplementaire.h"
#include <time.h>

struct tabBestM{
  unsigned long int score;
  dir best; 
};


//static void display_grid(grid g);
static long maximum_tile(grid g);
static tabBestM new_tabBestM();
static dir best_move(grid g);
static void boucle_best_move(tabBestM tab,dir def, grid g, grid test);
static dir mouvement_impossible(grid g);

int main(int argc,char **argv){
  srand(time(NULL));
  for (int j=0; j< 10;j++){
    grid g= new_grid();
    add_tile(g);
    add_tile(g);
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
    }
    printf("Tile max = %ld \n", maximum_tile(g));
    delete_grid(test);
    delete_grid(g);
  
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



static dir best_move(grid g){
  grid test1 = new_grid();
  tabBestM tab = new_tabBestM();
  boucle_best_move(tab,mouvement_impossible(test1),g,test1);
  dir b = tab.best;
  delete_grid(test1);
  return b;
}

static void boucle_best_move(tabBestM tab,dir def, grid g, grid test){
  for(dir i = UP;i<=RIGHT;i++){
    copy_grid(g,test);
    if(i != def)
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

  
/* static int* max_position(grid(g)){ */
/*   int t[2] = [0,0]; */
/*   long max_tile = 2; */
/*   for(int i = 0; i<GRID_SIDE; i++){ */
/*     for(int j = 0; j< GRID_SIDE; j++){ */
/*       if(get_tile(g, i, j)>max_tile){ */
/* 	max_tile = get_tile(g, i, j); */
/* 	t[0] = i; */
/* 	t[1] = j; */
/*       } */
/*     } */
/*   } */
/*   return t; */
/* } */

float board_position(grid g){
  int fusions_with_4 = 0;
  int fusions_with_2 = 0;
  for(int i = 0;i<4;i++){
    for(int j = 0;j<4;j++){
      if(get_tile(g,i,j)==0)
	continue;
      else{
	fusions_with_4 = number_of_fusions(g,4,i,j);
	fusions_with_2 = number_of_fusions(g,2,i,j);
      }
    }
  }
  return (fusion_with_4 + fusions_with_2)/empty_tiles(g);
}
 
int number_of_fusions(grid g,int num,int p1,int p2){
  int somme = 0;
    for(int i = 0;i<4;i++){
      if(i == p1)
	continue;
      if(get_tile(g,p1,i) == num)
	somme+=1;
    }
    for(int i = 0;i<4;i++){
      if(i==p2)
	continue;
      if(get_tile(g,i,p2)==num)
	somme+=1;
    }
    return somme;
}

int empty_tiles(grid g){
  int nb = 0;
  for(int i = 0; i<4;i++){
    for(int j = 0;j<4;j++){
      if(get_tile(g,i,j) == 0)
	nb += 1;
    }
  }
  return nb;
}

