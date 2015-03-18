#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "../src/grid.h"
#include "fonction-test.h"
#include <math.h>
  
/**
 * \file fonction-test.c
 * \brief Implémentation de fonction-test.h
 **/


bool test_new_grid(){
  grid g = new_grid();
  if(g == NULL)
    return false;
  delete_grid(g);
  return true;
}


bool test_get_tile(grid g){
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j<GRID_SIDE; j++)
      if(get_tile(g, i, j) != 0)
 	return false;
    return true;
}


bool test_set_tile(grid g){
  // Passage de toutes les cases à 2
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, 2);
  // Vérification que les cases sont à 2
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      if(get_tile(g, i, j) != 2)
 	return false;
  // Reset de la grid à zéro
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, 0);
  return true;
}


bool test_copy_grid(grid g){
  grid gCopy = new_grid();
  copy_grid(g, gCopy);  
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      if(get_tile(g, i, j) != get_tile(gCopy, i, j))
 	return false;
  delete_grid(gCopy);
  return true;
}


bool test_get_score_grid(grid g){
  if(grid_score(g) != 0)
    return false;
  return true;
}


bool test_game_over(grid g){
  int nbr = 1;
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++){
      set_tile(g, i, j, nbr);
      nbr++;
    }
  if(!game_over(g))
    return false;
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, 2);
  if(game_over(g))
    return false;
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, 0);
  set_tile(g, 0, 0, 2);
  if(game_over(g))
    return false;
  return true;
}


static bool check_case_empty(grid g,int debuti,int fini,int debutj,int finj){
  for(int i = debutj; i < finj; i++)
    for(int j = debuti; j < fini; j++)
        if(get_tile(g, i, j) != 0)
	  return false;
  return true;
}


bool test_do_move_up(grid g){
  bool resultat = true;
  
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, 0);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, i, 0, 1);
   
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, i, GRID_SIDE - 1, 1);
  do_move(g, UP);
  // On met des 2 sur la première ligne, et le reste de la grille est initialisé à 0
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, 0) != 2)
      return false;
  resultat = check_case_empty(g,1,GRID_SIDE,0,GRID_SIDE);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, i, GRID_SIDE - 1, 1);
  do_move(g, UP);
  // On met des 2 sur la première ligne, des 1 sur la deuxième ligne et le reste de la grille est initialisé à 0
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, 0) != 2)
      return false;
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, 1) != 1)
      return false;
  resultat = check_case_empty(g, 2, GRID_SIDE, 0, GRID_SIDE);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, i, GRID_SIDE - 1, 1);
  do_move(g, UP);
  // On met des 2 sur les deux premières lignes, et le reste de la grille est initialisé à 0
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, 0) != 2)
      return false;
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, 1) != 2)
      return false;
  resultat = check_case_empty(g, 2, GRID_SIDE, 0, GRID_SIDE);
  
  do_move(g, UP);
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, 0) != 3)
      return false;
  resultat = check_case_empty(g, 1, GRID_SIDE, 0, GRID_SIDE);
  return resultat;
}


bool test_do_move_down(grid g){
  bool resultat = true;
  // remise a 0 de la grille
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, 0);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, i, GRID_SIDE - 1, 1);
    
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, i, 0, 1);
  do_move(g, DOWN);
  // On met des 2 sur la première ligne, et le reste de la grille est initialisé à 0
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, GRID_SIDE - 1) != 2)
      return false;
  resultat = check_case_empty(g, 0, GRID_SIDE - 1, 0, GRID_SIDE);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, i, 0, 1);
  do_move(g, DOWN);
  // On met des 2 sur la première ligne, des 1 sur la deuxième ligne et le reste de la grille est initialisé à 0
  for(int i = 0; i <
	GRID_SIDE; i++)
    if(get_tile(g, i, GRID_SIDE - 1) != 2)
      return false;
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, GRID_SIDE - 2) != 1)
      return false;
  resultat = check_case_empty(g, 0, GRID_SIDE - 2, 0, GRID_SIDE);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, i, 0, 1);
  do_move(g, DOWN);
  // On met des 2 sur les deux premières lignes, et le reste de la grille est initialisé à 0
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, GRID_SIDE - 1) != 2)
      return false;
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, GRID_SIDE - 2) != 2)
      return false;
  resultat = check_case_empty(g, 0, GRID_SIDE - 2, 0, GRID_SIDE);
 
  do_move(g, DOWN);
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, GRID_SIDE - 1) != 3)
      return false;
  resultat = check_case_empty(g, 0, GRID_SIDE - 1, 0, GRID_SIDE);
  return resultat;
}


bool test_do_move_left(grid g){
  bool resultat = true;
  // remise a 0 de la grille
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, 0);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, 0, i, 1);
     
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, GRID_SIDE - 1, i, 1);
  do_move(g, LEFT);
  // On met des 2 sur la première ligne, et le reste de la grille est initialisé à 0
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, 0, i) != 2)
      return false;
  resultat = check_case_empty(g, 0, GRID_SIDE, 1, GRID_SIDE);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, GRID_SIDE - 1, i, 1);
  do_move(g, LEFT);
  // On met des 2 sur la première ligne, des 1 sur la deuxième ligne et le reste de la grille est initialisé à 0
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, 0, i) != 2)
      return false;
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, 1, i) != 1)
      return false;
  resultat = check_case_empty(g, 0, GRID_SIDE, 2, GRID_SIDE);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, GRID_SIDE - 1, i, 1);
  do_move(g, LEFT);
  // On met des 2 sur les deux premières lignes, et le reste de la grille est initialisé à 0
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, 0, i) != 2)
      return false;
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, 1, i) != 2)
      return false;
  resultat = check_case_empty(g, 0, GRID_SIDE, 2, GRID_SIDE);
  
  do_move(g, LEFT);
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, 0, i) != 3)
      return false;
  resultat = check_case_empty(g, 0, GRID_SIDE, 1, GRID_SIDE);
  return resultat;
}


bool test_do_move_right(grid g){
  bool resultat = true;
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, 0);
  
   
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, GRID_SIDE - 1, i, 1);
   
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, 0, i, 1);
  do_move(g, RIGHT);
  // On met des 2 sur la première ligne, et le reste de la grille est initialisé à 0
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, GRID_SIDE - 1, i) != 2)
      return false;
  resultat = check_case_empty(g, 0, GRID_SIDE, 0, GRID_SIDE - 1);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, 0, i, 1);
  do_move(g, RIGHT);
  // On met des 2 sur la première ligne, des 1 sur la deuxième ligne et le reste de la grille est initialisé à 0
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, GRID_SIDE - 1, i) != 2)
      return false;
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, GRID_SIDE - 2, i) != 1)
      return false;
  resultat = check_case_empty(g, 0, GRID_SIDE, 0, GRID_SIDE - 2);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, 0, i, 1);
  do_move(g, RIGHT);
  // On met des 2 sur les deux premières lignes, et le reste de la grille est initialisé à 0
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, GRID_SIDE - 1, i) != 2)
      return false;
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, GRID_SIDE - 2, i) != 2)
      return false;
  resultat = check_case_empty(g, 0, GRID_SIDE, 0, GRID_SIDE - 2);
 
  do_move(g, RIGHT);
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, GRID_SIDE - 1, i) != 3)
      return false;
  resultat = check_case_empty(g, 0, GRID_SIDE, 0, GRID_SIDE - 1);
  return resultat;
}


bool test_do_move_all(grid g){
  bool resultat = true;
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, 2); // On initialise toute la grille à 2
  do_move(g, UP);
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE - 2; j++)
      if(get_tile(g, i, j) != 3)
	return false;
  resultat = check_case_empty(g, 2, GRID_SIDE, 0, GRID_SIDE);
  do_move(g, DOWN);
  for(int j = 0; j < GRID_SIDE; j++)
    if(get_tile(g, j, GRID_SIDE-1) != 4)
      return false;
  resultat = check_case_empty(g, 0, GRID_SIDE - 1, 0, GRID_SIDE);
  do_move(g, LEFT);
  for(int j = 0; j < GRID_SIDE - 1; j++){
    if(j == 0 || j == 1){
      if(get_tile(g, j, GRID_SIDE - 1) != 5)
	return false;
    }
    else if(get_tile(g, j, GRID_SIDE - 1) != 0)
      return false;
  }
  resultat = check_case_empty(g, 0, GRID_SIDE - 1, 0, GRID_SIDE);

  do_move(g, RIGHT );
    for(int j = 0; j < GRID_SIDE - 3; j++){
      if( j == GRID_SIDE - 1){
	if(get_tile(g, j, GRID_SIDE - 1) != 6)
	  return false;
      }
      else if(get_tile(g, j, GRID_SIDE - 1) != 0)
	return false;
    }
    resultat = check_case_empty(g, 0, GRID_SIDE - 1, 0, GRID_SIDE);
  return resultat;
}


bool test_can_move(grid g){
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, 0); // On initialise toute la grille à 0
  if(can_move(g, UP) || can_move(g, DOWN) || can_move(g, RIGHT) || can_move(g, LEFT) )
    return false;
 
  set_tile(g, 2, 2, 2);
  if( !can_move(g, UP) || !can_move(g, DOWN) || !can_move(g, RIGHT) || !can_move(g, LEFT))
     return false;
  set_tile(g, 2, 2, 0);

  set_tile(g, 0, 3, 2); 
  set_tile(g, 1, 3, 2);
  if(!can_move(g, UP) || can_move(g, DOWN) || !can_move(g,RIGHT) || !can_move(g, LEFT))
    return false;
  set_tile(g, 1, 3, 4);
  set_tile(g, 2, 3, 2);
  set_tile(g, 3, 3, 4);
  if(!can_move(g, UP) || can_move(g, DOWN) || can_move(g,RIGHT) || can_move(g, LEFT))
    return false;
  set_tile(g, 2, 3, 4);
  if(!can_move(g, UP) || can_move(g, DOWN) || !can_move(g,RIGHT) || !can_move(g, LEFT))
    return false;
  set_tile(g, 0, 3, 0);
  set_tile(g, 1, 3, 0);
  set_tile(g, 2, 3, 0);
  set_tile(g, 3, 3, 0);

  set_tile(g, 0, 0, 2);
  set_tile(g, 1, 0, 2);
  if(can_move(g, UP) || !can_move(g, DOWN) || !can_move(g, RIGHT) || !can_move(g, LEFT))
    return false;
  set_tile(g, 1, 0, 0);

  set_tile(g, 0, 1, 2);
  if(!can_move(g, UP) || !can_move(g, DOWN) || !can_move(g, RIGHT) || can_move(g, LEFT))
    return false;
  
  set_tile(g, 0, 1, 4);
   if(can_move(g, UP) || !can_move(g, DOWN) || !can_move(g, RIGHT) || can_move(g, LEFT))
    return false;

   set_tile(g, 0, 2, 2);
   set_tile(g, 0, 3, 4);
   if(can_move(g, UP) || can_move(g, DOWN) || !can_move(g, RIGHT) || can_move(g, LEFT))
    return false;
   set_tile(g, 0, 3, 2);
   
   if(!can_move(g, UP) || !can_move(g, DOWN) || !can_move(g, RIGHT) || can_move(g, LEFT))
    return false;

  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, 0); // On initialise toute la grille à 0
  if(can_move(g, UP) || can_move(g, DOWN) || can_move(g, RIGHT) || can_move(g, LEFT))
    return false;
  for(int i = 0; i < GRID_SIDE - 1; i++)
    set_tile(g, 3, 1, 2);
  if(!can_move(g, UP))
     return false;
   if(!can_move(g, DOWN))
     return false;
   if(!can_move(g, LEFT))
     return false;
   if(can_move(g, RIGHT))
     return false;

   return true;  
}


bool test_add_tile(grid g){
  int rand4 = 0;
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, 0); // On initialise toute la grille à 0
  for(int k = 16; k > 0; k--)
    add_tile(g);
  for(int i = 0; i <  GRID_SIDE; i++)
    for(int j = 0; j <  GRID_SIDE; j++){
      if(get_tile(g, i, j) == 0)
	return false;
      if
	(get_tile(g, i, j) == 2)
	rand4 += 1;
    }
  if(rand4 < 1)
    return false;
  return true;
}


