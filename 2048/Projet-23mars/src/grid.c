#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "grid.h"

/**
 * \file grid.c
 * \brief Implémentation de grid.h
 * \author Ysabelle Emery, \n
 *         Jimmy Gouraud, \n
 *         Yordan Kirov, \n
 *         Sébastien Pouteau.
 * \date Licence 2 d'informatique - Mars 2015
 **/


/**
 * \brief Choix d'implémentaion de la structure:
 *    - tableau 2D de tile 
 *    - score de la grille
 **/
struct grid_s{
  tile **grid; 
  unsigned long int score;
};


// Déclaration des fonctions static
static void move(grid g, int i, int j, int a, int b);
static void fusion(grid g, int i, int j, int a, int b);
static bool possible(grid g, int i, int j, int a, int b);
static void set_grid_score(grid g, unsigned long int add_score);


/* ======================================
    IMPLEMENTATION DE L'INTERFACE GRID.H
   ====================================== */

grid new_grid(){
  grid g = malloc(sizeof(struct grid_s));
  assert(g != NULL);
  g->grid = malloc(sizeof(tile*) * GRID_SIDE);
  assert(g->grid != NULL);
  // Initialisation de chaque case du tableau à 0
  for(int i = 0; i < GRID_SIDE; i++){
    g->grid[i] = malloc(sizeof(tile) * GRID_SIDE);
    assert(g->grid[i] != NULL);
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, 0);
  }
  g->score = 0;
  return g;
}


void delete_grid(grid g){
  assert(g != NULL && g->grid != NULL);
  // Destruction du tableau grid
  for(int i = 0; i < GRID_SIDE; i++){       
    assert(g->grid[i] != NULL);     
    free(g->grid[i]);
   }
  free(g->grid);
  // Destruction de la structure grid
  free(g);
}


void copy_grid(grid src, grid dst){
  assert(src != NULL && dst != NULL);
  // Copie du tableau grid
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(dst, i, j, get_tile(src, i, j));
  dst->score = src->score;
}


unsigned long int grid_score(grid g){
  assert(g != NULL);
  return g->score;
}


tile get_tile(grid g, int x, int y){
  assert(g != NULL && g->grid != NULL);
  assert(0 <= x && x < GRID_SIDE && 0 <= y && y < GRID_SIDE);
  return g->grid[x][y];
}


void set_tile(grid g, int x, int y, tile t){
  assert(g != NULL && g->grid != NULL);
  assert(0 <= x && x < GRID_SIDE && 0 <= y && y < GRID_SIDE);
  g->grid[x][y] = t;
}


bool game_over(grid g){
  assert(g != NULL);
  return (can_move(g, UP) == false && can_move(g, LEFT) == false && can_move(g, RIGHT) == false && can_move(g, DOWN) == false);
}


bool can_move(grid g, dir d){
  assert(g != NULL);
  switch(d){
  case UP:
    return possible(g, 0, 0, 0, 1);
  case DOWN:
    return possible(g, 0, GRID_SIDE-1, 0, -1);
  case LEFT:
    return possible(g, 0, 0, 1, 0);
  case RIGHT:
    return possible(g, GRID_SIDE-1, 0, -1, 0);
  default:
    return false;
  }
}


void do_move(grid g, dir d){
  assert(g != NULL);
  switch(d){
  case UP:
    move(g, 0, 0, 0, 1);
    break;
  case DOWN:
    move(g, 0, GRID_SIDE-1, 0, -1);
    break;
  case LEFT:
    move(g, 0, 0, 1, 0);
    break;
  case RIGHT:
    move(g, GRID_SIDE-1, 0, -1, 0);
    break;
  default:
    break;
  }
}


void add_tile(grid g){
  assert(g != NULL);
  int cpt_case_empty = 0;
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      if(get_tile(g, i, j) == 0 )
	cpt_case_empty++;  

  int position_random = rand() % cpt_case_empty;
  int nbr_tile = rand() % 10;
  cpt_case_empty = 0;
  for(int i = 0; i < GRID_SIDE; i++){
    for(int j = 0; j < GRID_SIDE; j++){
      if(get_tile(g, i, j) == 0){
	if(cpt_case_empty == position_random){
	  if(nbr_tile == 9)
	    set_tile(g, i, j, 2);
	  else
	    set_tile(g, i, j, 1);
	}
	cpt_case_empty++;
      }
    }
  }
}


void play(grid g, dir d){
  assert(g != NULL);
  do_move(g, d);
  add_tile(g);
}


/* =====================================
    IMPLEMENTATION DES FONCTIONS STATIC
   ===================================== */

/**
 * \brief Fusionne deux cases dans toute la grille et met la deuxième case à 0
 * \param g grid
 * \param i1, j1 coordonnée de la première case
 * \param i2, j2 coordonnée de la deuxième case
 **/
static void fusion(grid g, int i1, int j1, int i2, int j2){
  assert(g != NULL);
  if(get_tile(g, i1, j1) == 0){
    set_tile(g, i1, j1, get_tile(g, i2, j2));
  }
  else{
    set_tile(g, i1, j1, get_tile(g, i1, j1) + 1);
  }  
  set_tile(g, i2, j2, 0);
}


/**
 * \brief Permet d'incrémenter deux variables ayant des incrémentations différentes.
 * \param i1, i2 les variables à incrémenter 
 * \param incrementationI1 incrémentation du premier paramètre
 * \param incrementationI2 incrémentation du deuxième paramètre
 **/
static void incrementation(int *i1, int *i2, int incrementationI1, int incrementationI2){
  *i1 += incrementationI1;
  *i2 += incrementationI2;
}


/**
 * \brief Déplace l'ensemble de la grille dans la direction voulue (en fonction des paramètres)
 * \param g grid visée
 * \param x indice de l'ordonnée de départ
 * \param y indice de l'abscisse de départ
 * \param addx variable d'incrémentation de l'ordonnée pour parcourir la grille
 * \param addy variable d'incrémentation de l'abscisse pour parcourir la grille
 * \pre les directions prérequis : UP    -> (grid, 0, 0, 0, 1)
 *                                 DOWN  -> (grid, 0, GRID_SIDE - 1, 0, -1)
 *                                 LEFT  -> (grid, 0, 0, 1, 0)
 *                                 RIGHT -> (grid, GRID_SIDE - 1, 0, -1, 0)
 **/
static void move(grid g, int x, int y, int addx, int addy){
  assert(g != NULL);
  // Traite toutes les lignes ou toutes les colonnes via x et j
  for(int cpt = 0; cpt < GRID_SIDE; cpt++){
    // Case actuelle
    int tmpx = x;
    int tmpy = y;
    // Case suivante dans le sens défini (devient case compteur)
    int cptx = x + addx;
    int cpty = y + addy;
    // Traite les lignes/colonnes en les parcourant avec cpti et cptj
    while(cptx < GRID_SIDE && cpty < GRID_SIDE && 0 <= cptx && 0 <= cpty){
      
      if((tmpx == cptx && tmpy == cpty) || get_tile(g, cptx, cpty) == 0)
	// On déplace la case compteur sur le case d'après
	incrementation(&cptx, &cpty, addx, addy);
      
      else if(get_tile(g, tmpx, tmpy) == 0)
	// On fusionne la case actuelle avec la case compteur (la case actuelle récupère la valeur de la case compteur)
	fusion(g, tmpx, tmpy, cptx, cpty);

      else if(get_tile(g, cptx, cpty) == get_tile(g, tmpx, tmpy)){
	// On fusionne la case actuelle avec la case compteur 
	fusion(g, tmpx, tmpy, cptx, cpty);
	set_grid_score(g, get_tile(g, tmpx, tmpy));
	// On déplace la case actuelle et la case compteur dans le sens défini
	incrementation(&tmpx, &tmpy, addx, addy);
	incrementation(&cptx, &cpty, addx, addy);
      }
      // On change la case actuelle sur la même ligne/colonne suivant le sens défini
      else
	incrementation(&tmpx, &tmpy, addx, addy);
    }
    // On change de ligne ou de colonne suivant le sens défini
    if(addx == -1 || addx == 1)
      y++;
    else
      x++;
  }
}


/**
 * \brief Teste sur l'ensemble de la grille si le deplacement dans la direction voulue est faisable (en fonction des paramètres) 
 * \param g grid visée
 * \param x indice de l'ordonnée de départ
 * \param y indice de l'abscisse de départ
 * \param addx variable d'incrémentation de l'ordonnée pour parcourir la grille
 * \param addy variable d'incrémentation de l'abscisse pour parcourir la grille
 * \ret true si possible, false sinon
 * \pre les directions prérequis : UP    -> (grid, 0, 0, 0, 1)
 *                                 DOWN  -> (grid, 0, GRID_SIDE - 1, 0, -1)
 *                                 LEFT  -> (grid, 0, 0, 1, 0)
 *                                 RIGHT -> (grid, GRID_SIDE - 1, 0, -1, 0)
 **/
static bool possible(grid g, int x,int y, int addx, int addy){
  assert(g != NULL);
  int tmpx = x;
  int tmpy = y;
  for(int cpt = 0; cpt < GRID_SIDE; cpt++){
    while(x < GRID_SIDE - addx && x >= 0 - addx && y < GRID_SIDE - addy && y >= 0 - addy){

      // Si deux cases collées sont indentiques alors return true
      if((get_tile(g, x, y) == get_tile(g, x + addx, y + addy) && get_tile(g, x, y) != 0))
	return true;

      // Si une case vide est suivie d'une case non vide alors return true
      else if(get_tile(g, x, y) == 0 && get_tile(g, x + addx, y + addy) != 0)
	return true;
      incrementation(&x, &y, addx, addy);
    }

    // Réinitialiser x ou y et incrémenter l'autre indice pour changer de ligne ou de colonne selon la direction
    if(addx == -1 || addx == 1)
      incrementation(&y, &x, 1, tmpx - x); 
    else
      incrementation(&x, &y, 1, tmpy - y); 
  }
  return false;
}


/**
 * \brief Ajoute un score au score de la grid
 * \param g grid 
 * \param add_score score à ajouter
 **/
static void set_grid_score(grid g, unsigned long int add_score){
  assert(g != NULL);
  g->score += pow(2, add_score);
}
