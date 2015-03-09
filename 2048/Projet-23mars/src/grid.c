#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "grid.h"

/*
Choix d'implémentaion de la structure:
 - tableau 2D de tile 
 - tableau 2D stockant les coordonnées des cases vides. (t[a][0] = abscisse ; t[a][1] = ordonnée)
 - nombre de cases vides
 - score de la grille
*/
struct grid_s{
  tile **grid; 
  unsigned long int score;
};

// Déclaration des fonctions static
static void move( grid g,int i, int j,int a, int b );
static void fusion (grid g,int i,int j,int a,int b);
static bool possible(grid g, int i,int j,int a,int b);


/* ======================================
    IMPLEMENTATION DE L'INTERFACE GRID.H
   ====================================== */

grid new_grid (){
  grid g = malloc(sizeof(struct grid_s));
  assert(g!= NULL);
  g->grid = malloc(sizeof(tile*)*GRID_SIDE);
  assert (g->grid !=NULL);
  // Initialisation de chaque case du tableau à 0
  for(int i=0; i< GRID_SIDE; i++){
    g->grid[i] = malloc(sizeof(tile)*GRID_SIDE);
    assert (g->grid[i] != NULL);
    for(int j=0; j< GRID_SIDE; j++)
      set_tile(g,i,j,0);
  }
  g->score = 0;
  return g;
}

void delete_grid (grid g){
  assert ( g!=NULL && g->grid!=NULL);
  // destruction du tableau grid
  for(int i = 0; i< GRID_SIDE; i++){       
    assert( g->grid[i] != NULL);     
    free(g->grid[i]);
    g->grid[i] = NULL;
  }
  free(g->grid);
  g->grid = NULL;
  //destruction de la structure grid
  free(g);
  g = NULL;
}

void copy_grid (grid src,grid dst){
  assert(src!=NULL && dst!=NULL);
  // copie du tableau grid
  for (int i=0;i<GRID_SIDE;i++)
    for(int j=0;j<GRID_SIDE;j++)
      set_tile(dst,i,j,get_tile(src,i,j));
  dst->score = src->score;
}

unsigned long int grid_score (grid g){
  assert(g!=NULL);
  return g->score;
}

tile get_tile(grid g,int x, int y){
  assert(g!=NULL && g->grid!=NULL);
  assert(0<=x && x<GRID_SIDE && 0<=y && y<GRID_SIDE);
  return g->grid[x][y];
}

void set_tile(grid g,int x, int y, tile t){
  assert(g!=NULL && g->grid!=NULL);
  assert(0<=x && x<GRID_SIDE && 0<=y && y<GRID_SIDE);
  g->grid[x][y]=t;
}

bool game_over(grid g){
  assert (g!=NULL);
  return (can_move(g,UP)==false && can_move(g,LEFT)==false && can_move(g,RIGHT)==false && can_move(g,DOWN)==false);
}

bool can_move(grid g,dir d){
  assert(g!=NULL);
  switch (d){
  case UP:
    return possible(g,0,0,1,0);
  case DOWN:
    return possible(g,GRID_SIDE-1,0,-1,0);
  case LEFT:
    return possible(g,0,0,0,1);
  case RIGHT:
    return possible(g,0,GRID_SIDE-1,0,-1);
  default:
    return false;
  }
}

void do_move(grid g, dir d){
  assert(g!=NULL);
  switch (d){
  case UP:
    move(g,0,0,1,0);
    break;
  case DOWN:
    move(g,GRID_SIDE-1,0,-1,0);
    break;
  case LEFT:
    move(g,0,0,0,1);
    break;
  case RIGHT:
    move(g,0,GRID_SIDE-1,0,-1);
    break;
  default:
    break;
  }
}

void add_tile (grid g){
  assert(g!=NULL);
  int cpt_case_empty = 0;
  for (int i = 0; i<GRID_SIDE;i++)
    for (int j = 0; j < GRID_SIDE; j++)
      if ( get_tile(g,i,j) == 0 )
	cpt_case_empty++;
  
  int position_random = rand()%cpt_case_empty;
  int nbr_tile = rand()%10;
  cpt_case_empty = 0;
  for (int i = 0; i<GRID_SIDE;i++){
    for (int j = 0; j < GRID_SIDE; j++){
      if ( get_tile(g,i,j) == 0 ){
	if (cpt_case_empty == position_random){
	  if (nbr_tile==9)
	    set_tile(g,i,j,2);
	  else
	    set_tile(g,i,j,1);
	}
	cpt_case_empty++;
      }
    }
  }
}

void play (grid g, dir d){
  assert(g!=NULL);
  if(can_move(g, d)){
    do_move(g, d);
    add_tile(g);
  }
}

void set_grid_score(grid g,unsigned long int add_score){
  assert(g!=NULL);
  g->score+=add_score;
}


/* =====================================
    IMPLEMENTATION DES FONCTIONS STATIC
   ===================================== */

/**
/brief fusion deux case et met la deuxième case a 0
/param1 grid
/param2,3 coordonnée de la première case
/param4,5 coordonnée de la deuxième case
 **/
static void fusion (grid g,int i1,int j1,int i2,int j2){
  assert(g!=NULL);
  if (get_tile(g,i1,j1)==0){
    set_tile(g,i1,j1,get_tile(g,i2,j2));
  }
  else{
    set_tile(g,i1,j1,get_tile(g,i1,j1)+1);
  }  
  set_tile(g,i2,j2,0);
}

/**
 * /brief incrementation permet d'incrémenter deux variables ayant des incrémentations différentes.
 * /param1,2 les variables a incrémenter 
 * /param3 incrementation du premier parametre
 * /param4 incrementation du deuxième parametre
 **/
static void incrementation(int *i1, int *i2, int incrementationI1, int incrementationI2){
  *i1+=incrementationI1;
  *i2+=incrementationI2;
}

/**
 * /brief  déplace l'ensemble de la grille dans la direction voulue (en fonction des paramètres)
 * /param1 grid visée
 * /param2 indice de l'ordonné de départ
 * /param3 indice de l'abscisse de départ
 * /param4 méthode d'indentation de l'ordonnée
 * /param4 méthode d'indentation de l'abscisse
 * /pre les directions prérequit UP (grid , 0, 0, 1, 0) DOWN (grid ,GRID_SIDE-1,0,-1,0) LEFT (grid,0,0,0,1); RIGHT (grid,0,GRID_SIDE-1,0,-1)
 **/
static void move(grid g,int i, int j,int indenti, int indentj){
  assert(g!=NULL);
  for (int cpt=0;cpt<GRID_SIDE;cpt++){
    int tmpi=i;
    int tmpj=j;
    int cpti=i+indenti;
    int cptj=j+indentj;
    while(cpti<GRID_SIDE && cptj<GRID_SIDE && 0<=cpti && 0<=cptj){
      if( (tmpi==cpti && tmpj==cptj) || get_tile(g,cpti,cptj)==0 )
	incrementation(&cpti,&cptj,indenti,indentj);
      else if(get_tile(g,tmpi,tmpj)==0)
	fusion(g,tmpi,tmpj,cpti,cptj);
      else if (get_tile(g,cpti,cptj)==get_tile(g,tmpi,tmpj)){
	fusion(g,tmpi,tmpj,cpti,cptj);
	set_grid_score(g,get_tile(g,tmpi,tmpj));
	incrementation(&tmpi,&tmpj,indenti,indentj);
	incrementation(&cpti,&cptj,indenti,indentj);
      }
      else
	incrementation(&tmpi,&tmpj,indenti,indentj);
    }
    if(indenti==-1 || indenti==1)
      j++;
    else
      i++;
  }
}

// teste s'il est possible de faire un déplacement dans une direction
static bool possible(grid g, int i,int j,int indenti,int indentj){
  assert(g!=NULL);
  int tmpi=i;
  int tmpj=j;
  for (int cpt=0;cpt<GRID_SIDE;cpt++){
    while(i<GRID_SIDE-indenti && i>=0-indenti && j<GRID_SIDE-indentj && j>=0-indentj){
      // si deux cases collés sont indentique alors return true
      if( (get_tile(g,i,j)==get_tile(g,i+indenti,j+indentj) && get_tile(g,i,j)!=0) )
	return true;
      // si une case vide est suivi d'une case non vide alors return true
      else if( get_tile(g,i,j)==0 && get_tile(g,i+indenti,j+indentj)!=0 )
	return true;
      incrementation(&i,&j,indenti,indentj);
    }
    // réinitialiser i ou j et incrémenter l'autre pour changer de ligne ou de colonne selon la direction
    if(indenti==-1 || indenti==1)
      incrementation (&j,&i,1,tmpi-i); 
    else
      incrementation (&i,&j,1,tmpj-j); 
  }
  return false;
}
