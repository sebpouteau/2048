#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "grid.h"

struct grid_s{
  tile **grid;
  int **case_empty;  //stock les coordonnées des cases vides dans un tableau 2D
  int nbr_case_empty;
  long score;
};

static void deplacement ( grid g,int i, int j,int a, int b );
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
  g->nbr_case_empty=16;
  add_tile(g);
  add_tile(g);

  g->score = 0;
  return g;
}


void delete_grid (grid g){
  assert ( g!=NULL && g->grid!=NULL);
  for(int i = 0; i< GRID_SIDE; i++){
    assert( g->grid[i] != NULL);
    free(g->grid[i]);
  }
  free(g->grid);
  free(g);
}


void copy_grid (grid src,grid dst){
  assert (src!=NULL && dst!=NULL);
  for (int i=0;i<GRID_SIDE;i++)
    for(int j=0;j<GRID_SIDE;j++)
      set_tile(dst,i,j,get_tile(src,i,j)); //utilisation des accesseurs pour lire et modifier.
}

unsigned long int grid_score (grid g){
  assert(g!=NULL);
  return g->score;
}
static void modif_grid_score(grid g,unsigned long int ajout_score){
  g->score+=ajout_score;
}

tile get_tile(grid g,int x, int y){
  assert( 0<=x && x<GRID_SIDE && 0<=y && y<GRID_SIDE); // verification si coordonnées sont dans la grille
  return g->grid[x][y];
}

void set_tile(grid g,int x, int y, tile t){
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
    deplacement(g,0,0,1,0);
    break;
  case DOWN:
    deplacement(g,3,0,-1,0);
    break;
  case LEFT:
    deplacement(g,0,0,0,1);
    break;
  case RIGHT:
    deplacement(g,0,3,0,-1);
    break;
  default:
    break;
  }
}



static void grid_case_empty(grid g){
  int a=0; //conteur tableau case vide
  for(int i=0; i<GRID_SIDE; i++){
    for(int j = 0; j< GRID_SIDE; j++){
      if(g->grid[i][j] == 0){
	g->case_empty[a][0]=i;
	g->case_empty[a][1]=j;
	a++;
      }
    }
  }
  g->nbr_case_empty=a;
}


void add_tile (grid g){
  grid_case_empty(g);
  int position_aleatoire = rand()%g->nbr_case_empty;
  int nombre_tile = rand()%10;
  int x = g->case_empty[position_aleatoire][0];
  int y = g->case_empty[position_aleatoire][1];
  if (nombre_tile==9)
    set_tile(g,x,y,pow(2,2));
  else
    set_tile(g,x,y,pow(2,1));

}


void play (grid g, dir d){
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

static void deplacement(grid g,int i, int j,int indenti, int indentj ){  
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
	modif_grid_score(g,get_tile(g,tmpi,tmpj));
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
