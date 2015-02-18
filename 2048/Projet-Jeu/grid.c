#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "grid.h"


struct grid_s{
  tile **grid;
  int **case_empty;  //stock les coordonnées des cases vides (ayant un 0) dans un tableau 2D
  int nbr_case_empty;//stock le nombre de 0 = nombre de case vide
  long score;
};

static void move( grid g,int i, int j,int a, int b );
static void fusion (grid g,int i,int j,int a,int b);
static void grid_case_empty(grid g);
static bool possible(grid g, int i,int j,int a,int b);

grid
new_grid (){
  grid g = malloc(sizeof(struct grid_s));
  assert(g); //verification existence
  assert (g->grid = malloc(sizeof(tile*)*GRID_SIDE));
  assert (g->case_empty = malloc(sizeof(int*)*GRID_SIDE*GRID_SIDE)); // initialisation tableau case vide.
  for(int i=0; i< GRID_SIDE; i++){
    assert (g->grid[i] = malloc(sizeof(tile)*GRID_SIDE));
    for(int j=0; j< GRID_SIDE; j++) // initilisation grille a zero
      set_tile(g,i,j,0);
  }
  for (int i=0; i<GRID_SIDE*GRID_SIDE;i++){
    assert (g->case_empty[i] = malloc(sizeof(int)*2));   // creation deuxieme colonne du tableau de case vide
  }
  g->nbr_case_empty=GRID_SIDE*GRID_SIDE;
  add_tile(g);
  add_tile(g);

  g->score = 0;
  return g;
}


void delete_grid (grid g){
  assert ( g!=NULL && g->grid!=NULL && g->case_empty!=NULL);
  for(int i = 0; i< GRID_SIDE; i++){
    assert( g->grid[i] != NULL);
    free(g->grid[i]);
  }
  for(int i=0; i< GRID_SIDE* GRID_SIDE; i++)
    free(g->case_empty[i]);
  free(g->case_empty);
  free(g->grid);
  free(g);
}


void copy_grid (grid src,grid dst){
  assert (src!=NULL && dst!=NULL);
  for (int i=0;i<GRID_SIDE;i++)
    for(int j=0;j<GRID_SIDE;j++)
      set_tile(dst,i,j,get_tile(src,i,j)); //utilisation des accesseurs pour lire et modifier.
  dst->score = src->score;
  dst->nbr_case_empty = src->nbr_case_empty;
  for(int i = 0; i< GRID_SIDE * GRID_SIDE; i++)
    for(int j=0; j <2; j++)
      dst-> case_empty[i][j] = src -> case_empty[i][j];
}

 

unsigned long int grid_score (grid g){//recuperation du score
  assert(g!=NULL);
  return g->score;
}
static void set_grid_score(grid g,unsigned long int add_score){//modification du score 
  assert(g!=NULL);
  g->score+=add_score;
}

tile get_tile(grid g,int x, int y){
  assert(g!=NULL);
  assert( 0<=x && x<GRID_SIDE && 0<=y && y<GRID_SIDE); // verification si coordonnées sont dans la grille
  return g->grid[x][y];
}

void set_tile(grid g,int x, int y, tile t){
  assert(g!=NULL);
  assert( 0<=x && x<GRID_SIDE && 0<=y && y<GRID_SIDE);// verification si coordonnées sont dans la grille
  g->grid[x][y]=t;
}


bool game_over(grid g){
  assert (g!=NULL);
  return (can_move(g,UP)==false && can_move(g,LEFT)==false && can_move(g,RIGHT)==false && can_move(g,DOWN)==false); //si aucun deplacement alors retourne true sinon retourne false
}

bool can_move(grid g,dir d){
  switch (d){
  case UP:
    return possible(g,0,0,1,0);
    break;
  case DOWN:
    return possible(g,3,0,-1,0);
    break;
  case LEFT:
    return possible(g,0,0,0,1);
    break;
  case RIGHT:
    return possible(g,0,3,0,-1);
    break;
  default:
    return false;
    break;
  }
}


void do_move(grid g, dir d){
  switch (d){
  case UP:
    move(g,0,0,1,0);
    break;
  case DOWN:
    move(g,3,0,-1,0);
    break;
  case LEFT:
    move(g,0,0,0,1);
    break;
  case RIGHT:
    move(g,0,3,0,-1);
    break;
  default:
    break;
  }
}

tile* get_grid_case_empty(grid g){
  return g->case_empty;
}

int get_nbr_case_empty(grid g){
  return g->nbr_case_empty;
}
   
static void grid_case_empty(grid g){ //parcours la grille et recupere les coordonnes des tiles nulles
  assert(g!=NULL && g->grid != NULL && g->case_empty !=NULL);
  int a=0; //conteur tableau case vide
  for(int i=0; i<GRID_SIDE; i++){
    for(int j = 0; j< GRID_SIDE; j++){
      if(get_tile(g, i, j) == 0){
	g->case_empty[a][0]=i;
	g->case_empty[a][1]=j;
	a++;
      }
    }
  }
  g->nbr_case_empty=a;
}


void add_tile (grid g){
  assert(g!=NULL);
  grid_case_empty(g);
  int position_random = rand()%g->nbr_case_empty;
  int nbr_tile = rand()%10;
  int x = g->case_empty[position_random][0];
  int y = g->case_empty[position_random][1];
  if (nbr_tile==9)
    set_tile(g,x,y,pow(2,2));
  else
    set_tile(g,x,y,pow(2,1));

}


void play (grid g, dir d){
  assert(g!=NULL);
  if(can_move(g, d)){
    do_move(g, d);
    add_tile(g);
  }
}

static void fusion (grid g,int i1,int j1,int i2,int j2){
  set_tile(g,i1,j1,get_tile(g,i1,j1)+get_tile(g,i2,j2));
  set_tile(g,i2,j2,0);
}
static void incrementation(int *i1, int *i2, int incrementationI1, int incrementationI2){
  *i1+=incrementationI1;
  *i2+=incrementationI2;
}

static void move(grid g,int i, int j,int indenti, int indentj ){
  for (int cpt=0;cpt<GRID_SIDE;cpt++){
    int tmpi=i;
    int tmpj=j;
    int cpti=i+indenti;
    int cptj=j+indentj;
    while (cpti<GRID_SIDE && cptj<GRID_SIDE && 0<=cpti && 0<=cptj){
      if ( (tmpi==cpti && tmpj==cptj) || get_tile(g,cpti,cptj)==0 ){
	incrementation(&cpti,&cptj,indenti,indentj);
      }
      else if (get_tile(g,tmpi,tmpj)==0){
	fusion(g,tmpi,tmpj,cpti,cptj);
      }
      else if (get_tile(g,cpti,cptj)==get_tile(g,tmpi,tmpj)){
	fusion(g,tmpi,tmpj,cpti,cptj);
	set_grid_score(g,get_tile(g,tmpi,tmpj));
	incrementation(&tmpi,&tmpj,indenti,indentj);
	incrementation(&cpti,&cptj,indenti,indentj);
      }
      else{
	incrementation(&tmpi,&tmpj,indenti,indentj);
      }
    }
    if (indenti==-1 || indenti==1)
      j++;
    else
      i++;
  }
}

static bool possible(grid g, int i,int j,int a,int b){
  int tmpi=i;
  int tmpj=j;
  for (int cpt=0;cpt<4;cpt++){
    while(i<4-a && i>=0-a && j<4-b && j>=0-b){
      if ((get_tile(g,i,j)==get_tile(g,i+a,j+b)&&get_tile(g,i,j)!=0)||(get_tile(g,i,j)==0 && get_tile(g,i+a,j+b))){
	return true;
      }
      i+=a;
      j+=b;
    }
    if (a==-1 || a==1){
      j++;
      i=tmpi;
    }
    else {
      i++;
      j=tmpj;
    }
  }
  return false;
}
