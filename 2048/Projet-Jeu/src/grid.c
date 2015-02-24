#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "grid.h"

/*
Choix d'implémentaion de la structure:
tableau 2D de tile 
tableau 2D de coordonnée (tableau de 2 colonnes contenant abscisse puis ordonnée). Il sert a stocker les coordonnées des cases vides
*/
struct grid_s{
  tile **grid;
  int **case_empty;    //stock les coordonnées des cases vides (ayant un 0) dans un tableau 2D
  int nbr_case_empty;  //stock le nombre de 0 = nombre de case vide
  unsigned long score;
};

static void move( grid g,int i, int j,int a, int b );
static void fusion (grid g,int i,int j,int a,int b);
static void grid_case_empty(grid g);
static bool possible(grid g, int i,int j,int a,int b);



/* ======================================
    IMPLEMENTATION DE L'INTERFACE GRID.H
   ====================================== */


grid new_grid (){
  grid g = malloc(sizeof(struct grid_s));
  assert(g);  //verification existence
  assert (g->grid = malloc(sizeof(tile*)*GRID_SIDE));
  assert (g->case_empty = malloc(sizeof(int*)*GRID_SIDE*GRID_SIDE)); // initialisation tableau case vide.
  for(int i=0; i< GRID_SIDE; i++){
    assert (g->grid[i] = malloc(sizeof(tile)*GRID_SIDE));
    for(int j=0; j< GRID_SIDE; j++) // initilisation grille à zéro
      set_tile(g,i,j,0);
  }
  for (int i=0; i<GRID_SIDE*GRID_SIDE;i++){
    assert (g->case_empty[i] = malloc(sizeof(int)*2));   // création deuxième colonne du tableau de case vide
  }
  grid_case_empty(g); // mise à 0 des cases du tableau de case vide  
  g->nbr_case_empty=GRID_SIDE*GRID_SIDE;
  g->score = 0;
  return g;
}

void delete_grid (grid g){
  assert ( g!=NULL && g->grid!=NULL && g->case_empty!=NULL);
  // destruction du tableau grid
  for(int i = 0; i< GRID_SIDE; i++){       
    assert( g->grid[i] != NULL);     
    free(g->grid[i]);
  }
  free(g->grid);
  // destruction tableau de case vide
  for(int i=0; i< GRID_SIDE* GRID_SIDE; i++)
    free(g->case_empty[i]);
  free(g->case_empty);
  //destruction de la structure grid
  free(g);
}

void copy_grid (grid src,grid dst){
  assert(src!=NULL && dst!=NULL);
  // copie du tableau grid
  for (int i=0;i<GRID_SIDE;i++)
    for(int j=0;j<GRID_SIDE;j++)
      set_tile(dst,i,j,get_tile(src,i,j)); //utilisation des accesseurs pour lire et modifier.
  dst->score = src->score;
  dst->nbr_case_empty = src->nbr_case_empty;
  // copie du tableau de case vide
  for(int i = 0; i< GRID_SIDE * GRID_SIDE; i++)
    for(int j=0; j <2; j++)
      dst-> case_empty[i][j] = src -> case_empty[i][j];
}

// récupération du score
unsigned long int grid_score (grid g){
  assert(g!=NULL);
  return g->score;
}

tile get_tile(grid g,int x, int y){
  assert(g!=NULL && g->grid!=NULL);
  assert( 0<=x && x<GRID_SIDE && 0<=y && y<GRID_SIDE); // verification si coordonnées sont dans la grille
  return g->grid[x][y];
}

void set_tile(grid g,int x, int y, tile t){
  assert(g!=NULL && g->grid!=NULL);
  assert( 0<=x && x<GRID_SIDE && 0<=y && y<GRID_SIDE);// verification si coordonnées sont dans la grille
  g->grid[x][y]=t;
}

bool game_over(grid g){
  assert (g!=NULL);
  //si aucun deplacement alors return true sinon return false
  return (can_move(g,UP)==false && can_move(g,LEFT)==false && can_move(g,RIGHT)==false && can_move(g,DOWN)==false);
}

bool can_move(grid g,dir d){
  assert(g!=NULL);
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
  assert(g!=NULL);
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


/* ========================================
    IMPLEMENTATION DE L'INTERFACE GRIDIA.H
   ======================================== */

// permet de recuperer le tableau de case vide (interface gridIA.h)
int** get_grid_case_empty(grid g){
  assert(g!=NULL && g->case_empty!=NULL);
  return g->case_empty;
}

// permet de recuperer le nombre de case vide (interface gridIA.h)
int get_nbr_case_empty(grid g){
  assert(g!=NULL);
  return g->nbr_case_empty;
}


/* ====================================
    IMPLEMENTATION DES FONCTION STATIC
   ==================================== */

// permet de modifier le score 
static void set_grid_score(grid g,unsigned long int add_score){
  assert(g!=NULL);
  g->score+=add_score;
}

//parcours la grille et récupère les coordonnées des tiles vides et copie dans le tableau de case vide
static void grid_case_empty(grid g){ 
  assert(g!=NULL && g->grid != NULL && g->case_empty !=NULL);
  int a=0; //compteur tableau case vide
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

/* 
fusion permet de fusionner deux cases d'une grid
fusionne [i1][j1] et [i2][j2] dans [i1][i2] et met un 0 dans l'autre case
*/
static void fusion (grid g,int i1,int j1,int i2,int j2){
  assert(g!=NULL);
  set_tile(g,i1,j1,get_tile(g,i1,j1)+get_tile(g,i2,j2));
  set_tile(g,i2,j2,0);
}

/*
incrémentation permet d'incrémenter deux variables avec des incrémentation differentes
*/
static void incrementation(int *i1, int *i2, int incrementationI1, int incrementationI2){
  *i1+=incrementationI1;
  *i2+=incrementationI2;
}

/*
move permet de déplacer la grille selon les paramètres passés
c'est-à-dire, "indenti" va indenter i selon ce que l'on veux si on part de la fin ou du début pareil pour "indentj"
Ce qui permet de deplacer la grille dans toutes les directions 
*/
static void move(grid g,int i, int j,int indenti, int indentj){
  assert(g!=NULL);
  for (int cpt=0;cpt<GRID_SIDE;cpt++){
    int tmpi=i;
    int tmpj=j;
    int cpti=i+indenti;
    int cptj=j+indentj;
    while (cpti<GRID_SIDE && cptj<GRID_SIDE && 0<=cpti && 0<=cptj){
      // si compteur == tmpi alors indenter les compteurs 
      // si le compteur est sur 0 alors on indente le curseur
      if ( (tmpi==cpti && tmpj==cptj) || get_tile(g,cpti,cptj)==0 ){
	incrementation(&cpti,&cptj,indenti,indentj);
      }
      // si les tmp sont sur une case 0 alors on fusionne le cpt avec tmp 
      else if (get_tile(g,tmpi,tmpj)==0){
	fusion(g,tmpi,tmpj,cpti,cptj);
      }
      // si la case tmp == case cpt et si les cases sont différentes de 0, alors on fusionne les deux cases puis on change le score
      // on avance d'une case cpt et tmp avec incrémentation
      else if (get_tile(g,cpti,cptj)==get_tile(g,tmpi,tmpj)){
	fusion(g,tmpi,tmpj,cpti,cptj);
	set_grid_score(g,get_tile(g,tmpi,tmpj));
	incrementation(&tmpi,&tmpj,indenti,indentj);
	incrementation(&cpti,&cptj,indenti,indentj);
      }
      // les cases ne sont pas identiques et cpt est différent de 0 donc on avance tmp 
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

/*
possible renvoie true si il est possible de faire un déplacement dans une direction et false sinon
*/
static bool possible(grid g, int i,int j,int indenti,int indentj){
  assert(g!=NULL);
  int tmpi=i;
  int tmpj=j;
  for (int cpt=0;cpt<GRID_SIDE;cpt++){
    while(i<GRID_SIDE-indenti && i>=0-indenti && j<GRID_SIDE-indentj && j>=0-indentj){
      // si deux cases se suivent et sont indentique alors possible = true
      // si une case vide suivi d'une case non vide alors possible = true
      if ((get_tile(g,i,j)==get_tile(g,i+indenti,j+indentj)&&get_tile(g,i,j)!=0)||(get_tile(g,i,j)==0 && get_tile(g,i+indenti,j+indentj))){
	return true;
      }
      // sinon on incrémente i et j en fonction de la direction avec indenti indentj
      incrementation(&i,&j,indenti,indentj);
    }
    // selon direction réinitialiser i ou j
    // incrémenter l'autre 1 pour changer de ligne ou de colonne selon la direction
    if (indenti==-1 || indenti==1){
      incrementation (&j,&i,1,tmpi-i); 
    }
    else {
      incrementation (&i,&j,1,tmpj-j); 
     }
  }
  return false;
}
