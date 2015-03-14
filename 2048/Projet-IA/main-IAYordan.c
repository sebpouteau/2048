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


//static void display_grid(grid g);
static long maximum_tile(grid g);
//static tabBestM new_tabBestM();
static dir best_move(grid g,dir i,int compt,dir best,unsigned long int score);
static unsigned long int best_score(grid g);
static bool fusion_possible(grid g,int p1,int p2);
static int empty_tiles(grid g);
static float opponent_chance(grid g);

int main(int argc,char **argv){
  srand(time(NULL));
  for (int j=0; j< 10;j++){
    grid g= new_grid();
    add_tile(g);
    add_tile(g);
    while(!game_over(g)){
      play(g,best_move(g,UP,0,0,0));
    }
      
    printf("Tile max = %ld \n", maximum_tile(g));
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




//static tabBestM new_tabBestM(){
//tabBestM tab;
  //assert(tab);
  //tab.score = 0;
  //tab.best = 0;
  //return tab;
//}



static dir best_move(grid g,dir i,int compt,dir best,unsigned long int score){
  if(!can_move(g,i))
    return best_move(g,i++,compt++,best,score);
  if(compt == 4)
    return best;
  grid test = new_grid();
  copy_grid(g,test);
  do_move(test,i);
  unsigned long int new_score = grid_score(test);
  unsigned long int fin_score = 0;
  for(int i1 = 0;i1<4;i1++){    //corps de la fonction opponent_chance();
    for(int j1 = 0; j1<4;j1++){
      if(get_tile(test,i1,j1) == 0){
	set_tile(test,i1,j1,2);
	if(best_score(test)>new_score)
	  new_score = best_score(test);
	set_tile(test,i1,j1,0);
      }
      if(get_tile(test,i1,j1) == 0){
	set_tile(test,i1,j1,4);
	if(best_score(test)>new_score)
	  new_score = best_score(test);
	set_tile(test,i1,j1,0);
      }
    }
  }

  fin_score = new_score/opponent_chance(test);
  if(fin_score > score)
    return best_move(g,++i,compt++,i,fin_score);
  return best_move(g,i++,compt++,best,score);
}

static unsigned long int best_score(grid g){
  grid test = new_grid();
  unsigned long int score = grid_score(g);
  for(dir i = UP;i<=RIGHT;i++){
    copy_grid(g,test);
    if(can_move(test,i)){
      do_move(test,i);
      if(grid_score(test) > score)
	score = grid_score(test);
    }
	
  }
  return score;
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


static float opponent_chance(grid g){
  int fusions_with_4 = 0;
  int fusions_with_2 = 0;
  for(int i = 0;i<4;i++){
    for(int j = 0;j<4;j++){
      if(get_tile(g,i,j)==0){
	set_tile(g,i,j,2);
	if(fusion_possible(g,i,j))
	  fusions_with_2 += 1;
        set_tile(g,i,j,0);
      }
      if(get_tile(g,i,j) == 0){
	set_tile(g,i,j,4);
	if(fusion_possible(g,i,j))
	   fusions_with_4 += 1;
	set_tile(g,i,j,0);
	
      }   
    }
  }
  return (fusions_with_4 + (fusions_with_2 * 9))/(empty_tiles(g)*10);
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

static bool fusion_possible(grid g,int p1,int p2){
  for(int i = p1;i<3;i++){
    if(get_tile(g,i,p2+1) != get_tile(g,p1,p2) && get_tile(g,i,p2+1) != 0)
      return false;
    if(get_tile(g,i,p2+1) == 0)
      continue;
    if(get_tile(g,i,p2) == get_tile(g,i,p2+1))
      return true;
  }
  for(int i = p1;i>0;i--){
    if(get_tile(g,i,p2-1) != get_tile(g,p1,p2) && get_tile(g,i,p2-1) != 0)
      return false;
    if(get_tile(g,i,p2-1) == 0)
      continue;
    if(get_tile(g,i,p2) == get_tile(g,i,p2-1))
      return true;
  }
  for(int i = p2;i<4;i++){
    if(get_tile(g,p1+1,i) != get_tile(g,p1,p2) && get_tile(g,p1+1,i) != 0)
      return false;
    if(get_tile(g,p1+1,i) == 0)
      continue;
    if(get_tile(g,p2,i) == get_tile(g,p1+1,i))
      return true;
  }
  for(int i = p2;i>0;i--){
    if(get_tile(g,p1-1,i) != get_tile(g,p1,p2) && get_tile(g,p1-1,i) != 0)
      return false;
    if(get_tile(g,p1-1,i) == 0)
      continue;
    if(get_tile(g,p1,i) == get_tile(g,p1-1,i))
      return true;
  }
  return false;
}
