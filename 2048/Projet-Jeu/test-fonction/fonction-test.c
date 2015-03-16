#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "../src/grid.h"
#include "fonction-test.h"
#include <math.h>
  

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
  //passage de toute les cases à 2
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, 2);
  //verification que les cases sont à 2
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      if(get_tile(g, i, j) != 2)
 	return false;
  //reset de la grid a zero
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
  for(int i = debuti; i < fini; i++)
    for(int j = debutj; j < finj; j++)
        if(get_tile(g, i, j) != 0)
	  return false;
  return true;
}


bool test_do_move_up(grid g){
  bool resultat = true;
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, 0);
  do_move(g, UP);
  //cas mouvement de la grille vide
  resultat = check_case_empty(g, 0, GRID_SIDE, 0, GRID_SIDE);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, 0, i, 1);
  do_move(g, UP);
  // des 2 sur toute la 1ere ligne et des 0 partout ailleur
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, 0, i) != 1)
      return false;
  resultat = check_case_empty(g, 1, GRID_SIDE, 0, GRID_SIDE);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, GRID_SIDE - 1, i, 1);
  do_move(g, UP);
  // des 2 sur toute la 1ere ligne et des 0 partout ailleur
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, 0, i) != 2)
      return false;
  resultat = check_case_empty(g,1,GRID_SIDE,0,GRID_SIDE);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, GRID_SIDE - 1, i, 1);
  do_move(g, UP);
  // des 4 sur toute la 1ere ligne et 2 sur la deuxieme ligne et des 0 partout ailleur
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, 0, i) != 2)
      return false;
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, 1, i) != 1)
      return false;
  resultat = check_case_empty(g, 2, GRID_SIDE, 0, GRID_SIDE);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, GRID_SIDE - 1, i, 1);
  do_move(g, UP);
  // des 4 sur toute la 1ere ligne et 4 sur la deuxieme ligne et des 0 partout ailleur
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, 0, i) != 2)
      return false;
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, 1, i) != 2)
      return false;
  resultat = check_case_empty(g, 2, GRID_SIDE, 0, GRID_SIDE);
  
  do_move(g, UP);
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, 0, i) != 3)
      return false;
  resultat = check_case_empty(g, 1, GRID_SIDE, 0, GRID_SIDE);
  return resultat;
}


bool test_do_move_down(grid g){
  bool resultat = true;
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, 0);
  do_move(g, DOWN);
  //cas mouvement de la grille vide
  resultat = check_case_empty(g, 0, GRID_SIDE, 0, GRID_SIDE);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, GRID_SIDE - 1, i, 1);
  do_move(g, DOWN);
  // des 2 sur toute la 1ere ligne et des 0 partout ailleur
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, GRID_SIDE - 1, i) != 1)
      return false;
  resultat = check_case_empty(g, 0, GRID_SIDE - 1, 0, GRID_SIDE);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, 0, i, 1);
  do_move(g, DOWN);
  // des 2 sur toute la 1ere ligne et des 0 partout ailleur
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, GRID_SIDE - 1, i) != 2)
      return false;
  resultat = check_case_empty(g, 0, GRID_SIDE - 1, 0, GRID_SIDE);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, 0, i, 1);
  do_move(g, DOWN);
  // des 4 sur toute la 1ere ligne et 2 sur la deuxieme ligne et des 0 partout ailleur
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, GRID_SIDE - 1, i) != 2)
      return false;
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, GRID_SIDE - 2, i) != 1)
      return false;
  resultat = check_case_empty(g, 0, GRID_SIDE - 2, 0, GRID_SIDE);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, 0, i, 1);
  do_move(g, DOWN);
  // des 4 sur toute la 1ere ligne et 4 sur la deuxieme ligne et des 0 partout ailleur
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, GRID_SIDE - 1, i) != 2)
      return false;
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, GRID_SIDE - 2, i) != 2)
      return false;
  resultat = check_case_empty(g, 0, GRID_SIDE - 2, 0, GRID_SIDE);
 
  do_move(g, DOWN);
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, GRID_SIDE - 1, i) != 3)
      return false;
  resultat = check_case_empty(g, 0, GRID_SIDE - 1, 0, GRID_SIDE);
  return resultat;
}


bool test_do_move_left(grid g){
  bool resultat = true;
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, 0);
  do_move(g, LEFT);
  //cas mouvement de la grille vide
  resultat = check_case_empty(g, 0, GRID_SIDE, 0, GRID_SIDE);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, i, 0, 1);
  do_move(g, LEFT);
  // des 2 sur toute la 1ere ligne et des 0 partout ailleur
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, 0) != 1)
      return false;
  resultat = check_case_empty(g, 0, GRID_SIDE, 1, GRID_SIDE);
   
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, i, GRID_SIDE - 1,1);
  do_move(g, LEFT);
  // des 2 sur toute la 1ere ligne et des 0 partout ailleur
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, 0) != 2)
      return false;
  resultat = check_case_empty(g, 0, GRID_SIDE, 1, GRID_SIDE);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, i, GRID_SIDE - 1, 1);
  do_move(g, LEFT);
  // des 4 sur toute la 1ere ligne et 2 sur la deuxieme ligne et des 0 partout ailleur
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, 0) != 2)
      return false;
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, 1) != 1)
      return false;
  resultat = check_case_empty(g, 0, GRID_SIDE, 2, GRID_SIDE);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, i, GRID_SIDE - 1, 1);
  do_move(g, LEFT);
  // des 4 sur toute la 1ere ligne et 4 sur la deuxieme ligne et des 0 partout ailleur
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, 0) != 2)
      return false;
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, 1) != 2)
      return false;
  resultat = check_case_empty(g, 0, GRID_SIDE, 2, GRID_SIDE);
  
  do_move(g, LEFT);
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, 0) != 3)
      return false;
  resultat = check_case_empty(g, 0, GRID_SIDE, 1, GRID_SIDE);
  return resultat;
}


bool test_do_move_right(grid g){
  bool resultat = true;
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, 0);
  do_move(g, RIGHT);
  //cas mouvement de la grille vide
  resultat = check_case_empty(g, 0, GRID_SIDE, 0, GRID_SIDE);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, i, GRID_SIDE - 1, 1);
  do_move(g, RIGHT);
  // des 2 sur toute la 1ere ligne et des 0 partout ailleur
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, GRID_SIDE - 1) != 1)
      return false;
  resultat = check_case_empty(g, 0, GRID_SIDE, 0, GRID_SIDE - 1); 
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, i, 0, 1);
  do_move(g, RIGHT);
  // des 2 sur toute la 1ere ligne et des 0 partout ailleur
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, GRID_SIDE - 1) != 2)
      return false;
  resultat = check_case_empty(g, 0, GRID_SIDE, 0, GRID_SIDE - 1);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, i, 0, 1);
  do_move(g, RIGHT);
  // des 4 sur toute la 1ere ligne et 2 sur la deuxieme ligne et des 0 partout ailleur
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, GRID_SIDE - 1) != 2)
      return false;
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, GRID_SIDE - 2) != 1)
      return false;
  resultat = check_case_empty(g, 0, GRID_SIDE, 0, GRID_SIDE - 2);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, i, 0, 1);
  do_move(g, RIGHT);
  // des 4 sur toute la 1ere ligne et 2 sur la deuxieme ligne et des 0 partout ailleur
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, GRID_SIDE - 1) != 2)
      return false;
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, GRID_SIDE - 2) != 2)
      return false;
  resultat = check_case_empty(g, 0, GRID_SIDE, 0, GRID_SIDE - 2);
 
  do_move(g, RIGHT);
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, GRID_SIDE - 1) != 3)
      return false;
  resultat = check_case_empty(g, 0, GRID_SIDE, 0, GRID_SIDE - 1);
  return resultat;
}


bool test_do_move_all(grid g){
  bool resultat = true;
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, 2); // on initialise tout à 2
  do_move(g, UP);
  for(int i = 0; i < GRID_SIDE - 2; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      if(get_tile(g, i, j) != 3)
	return false;
  resultat = check_case_empty(g, 2, GRID_SIDE, 0, GRID_SIDE);
  do_move(g, DOWN);
  for(int j = 0; j < GRID_SIDE; j++)
      if(get_tile(g, GRID_SIDE-1, j) != 4)
	return false;
  resultat = check_case_empty(g, 0, GRID_SIDE - 1, 0, GRID_SIDE);
  do_move(g, LEFT);
  for(int j = 0; j < GRID_SIDE - 1; j++){
    if(j == 0 || j == 1){
      if(get_tile(g, GRID_SIDE - 1, j) != 5)
	return false;
    }
    else if(get_tile(g, GRID_SIDE - 1, j) != 0)
      return false;
  }
  resultat = check_case_empty(g, 0, GRID_SIDE - 1, 0, GRID_SIDE);

  do_move(g, RIGHT );
    for(int j = 0; j < GRID_SIDE - 3; j++){
      if( j == GRID_SIDE - 1){
	if(get_tile(g, GRID_SIDE - 1, j) != 6)
	  return false;
      }
      else if(get_tile(g, GRID_SIDE - 1, j) != 0)
	return false;
    }
    resultat = check_case_empty(g, 0, GRID_SIDE - 1, 0, GRID_SIDE);
  return resultat;
}


bool test_can_move(grid g){
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, 0); // on initialise tout à 0
  if(can_move(g, UP) || can_move(g, DOWN) || can_move(g, RIGHT) || can_move(g, LEFT) )
    return false;
 
  set_tile(g, 2, 2, 2);
  if( !can_move(g, UP) || !can_move(g, DOWN) || !can_move(g, RIGHT) || !can_move(g, LEFT))
     return false;
  set_tile(g, 2, 2, 0);

  set_tile(g, 3, 0, 2); 
  set_tile(g, 3, 1, 2);
  if(!can_move(g, UP) || can_move(g, DOWN) || !can_move(g,RIGHT) || !can_move(g, LEFT))
    return false;
  set_tile(g, 3, 1, 4);
  set_tile(g, 3, 2, 2);
  set_tile(g, 3, 3, 4);
  if(!can_move(g, UP) || can_move(g, DOWN) || can_move(g,RIGHT) || can_move(g, LEFT))
    return false;
  set_tile(g, 3, 2, 4);
  if(!can_move(g, UP) || can_move(g, DOWN) || !can_move(g,RIGHT) || !can_move(g, LEFT))
    return false;
  set_tile(g, 3, 0, 0);
  set_tile(g, 3, 1, 0);
  set_tile(g, 3, 2, 0);
  set_tile(g, 3, 3, 0);

  set_tile(g, 0, 0, 2);
  set_tile(g, 0, 1, 2);
  if(can_move(g, UP) || !can_move(g, DOWN) || !can_move(g, RIGHT) || !can_move(g, LEFT))
    return false;
  set_tile(g, 0, 1, 0);

  set_tile(g, 1, 0, 2);
  if(!can_move(g, UP) || !can_move(g, DOWN) || !can_move(g, RIGHT) || can_move(g, LEFT))
    return false;
  
  set_tile(g, 1, 0, 4);
   if(can_move(g, UP) || !can_move(g, DOWN) || !can_move(g, RIGHT) || can_move(g, LEFT))
    return false;

   set_tile(g, 2, 0, 2);
   set_tile(g, 3, 0, 4);
   if(can_move(g, UP) || can_move(g, DOWN) || !can_move(g, RIGHT) || can_move(g, LEFT))
    return false;
   set_tile(g, 3, 0, 2);
   
   if(!can_move(g, UP) || !can_move(g, DOWN) || !can_move(g, RIGHT) || can_move(g, LEFT))
    return false;

  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, 0); // on initialise tout à 0
  if(can_move(g, UP) || can_move(g, DOWN) || can_move(g, RIGHT) || can_move(g, LEFT))
    return false;
  for(int i = 0; i < GRID_SIDE - 1; i++)
     set_tile(g, i, 3, 2);
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
      set_tile(g, i, j, 0); // on initialise tout à 0
  for(int k = 16; k > 0; k--)
    add_tile(g);
  for(int i = 0; i <  GRID_SIDE; i++)
    for(int j = 0; j <  GRID_SIDE; j++){
      if(get_tile(g, i, j) == 0)
	return false;
      if(get_tile(g, i, j) == 2)
	rand4 += 1;
    }
  if(rand4 < 1)
    return false;
  return true;
}


