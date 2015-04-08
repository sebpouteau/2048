#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <grid.h>
#include <fonction-test.h>
#include <math.h>

#define END_GRID GRID_SIDE-1
#define HALF_GRID_SIDE (int)GRID_SIDE/2
#define EMPTY_TILE 0
#define TILE_2 1
#define TILE_4 2
#define TILE_8 3
#define TILE_16 4
#define TILE_32 5  


/**
 * \file fonction-test.c
 * \brief Implementation de fonction-test.h
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
      if(get_tile(g, i, j) != EMPTY_TILE )
 	return false;
    return true;
}


bool test_set_tile(grid g){
  // Passage de toutes les cases a 2
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, TILE_2);
  // Verification que les cases sont a 2
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      if(get_tile(g, i, j) != TILE_2)
 	return false;
  // Reset de la grid a zero
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, EMPTY_TILE);
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
  // On remplis la grille de case differentes pour evite les possibilites de deplacements 
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
      set_tile(g, i, j, TILE_2);
  if(game_over(g))
    return false;
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, EMPTY_TILE);
  set_tile(g, 0, 0, TILE_2);
  if(game_over(g))
    return false;
  return true;
}


static bool check_case_empty(grid g,int debuti,int fini,int debutj,int finj){
  for(int i = debutj; i < finj; i++)
    for(int j = debuti; j < fini; j++)
        if(get_tile(g, i, j) != EMPTY_TILE)
	  return false;
  return true;
}


bool test_do_move_up(grid g){
  bool result = true;
  
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, EMPTY_TILE);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, i, 0, TILE_2);
   
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, i, END_GRID, TILE_2);
  do_move(g, UP);
  // On met des 2 sur la premiere ligne, et le reste de la grille est initialise a 0
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, 0) != TILE_4)
      return false;
  result = check_case_empty(g,1,GRID_SIDE,0,GRID_SIDE);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, i, END_GRID, TILE_2);
  do_move(g, UP);
  // On met des 2 sur la premiere ligne, des 1 sur la deuxieme ligne et le reste de la grille est initialise a 0
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, 0) != TILE_4)
      return false;
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, 1) != TILE_2)
      return false;
  result = check_case_empty(g, 2, GRID_SIDE, 0, GRID_SIDE);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, i, END_GRID, TILE_2);
  do_move(g, UP);
  // On met des 2 sur les deux premieres lignes, et le reste de la grille est initialise a 0
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, 0) != TILE_4)
      return false;
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, 1) != TILE_4)
      return false;
  result = check_case_empty(g, 2, GRID_SIDE, 0, GRID_SIDE);
  
  do_move(g, UP);
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, 0) != TILE_8)
      return false;
  result = check_case_empty(g, 1, GRID_SIDE, 0, GRID_SIDE);
  return result;
}


bool test_do_move_down(grid g){
  bool result = true;
  // remise a 0 de la grille
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, EMPTY_TILE);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, i, END_GRID, TILE_2);
    
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, i, 0, TILE_2);
  do_move(g, DOWN);
  // On met des 2 sur la premiere ligne, et le reste de la grille est initialise a 0
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, END_GRID) != TILE_4)
      return false;
  result = check_case_empty(g, 0, GRID_SIDE - 1, 0, GRID_SIDE);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, i, 0, TILE_2);
  do_move(g, DOWN);
  // On met des 2 sur la premiere ligne, des 1 sur la deuxieme ligne et le reste de la grille est initialise a 0
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, END_GRID) != TILE_4)
      return false;
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, END_GRID - 1) != TILE_2)
      return false;
  result = check_case_empty(g, 0, END_GRID - 1, 0, GRID_SIDE);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, i, 0, TILE_2);
  do_move(g, DOWN);
  // On met des 2 sur les deux premieres lignes, et le reste de la grille est initialise a 0
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, END_GRID) != TILE_4)
      return false;
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, END_GRID - 1) != TILE_4)
      return false;
  result = check_case_empty(g, 0, GRID_SIDE - 2, 0, GRID_SIDE);
 
  do_move(g, DOWN);
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, i, GRID_SIDE - 1) != TILE_8)
      return false;
  result = check_case_empty(g, 0, GRID_SIDE - 1, 0, GRID_SIDE);
  return result;
}


bool test_do_move_left(grid g){
  bool result = true;
  // remise a 0 de la grille
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, EMPTY_TILE);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, 0, i, TILE_2);
     
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, END_GRID, i, TILE_2);
  do_move(g, LEFT);
  // On met des 2 sur la premiere ligne, et le reste de la grille est initialise a 0
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, 0, i) != TILE_4)
      return false;
  result = check_case_empty(g, 0, GRID_SIDE, 1, GRID_SIDE);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, END_GRID, i, TILE_2);
  do_move(g, LEFT);
  // On met des 2 sur la premiere ligne, des 1 sur la deuxieme ligne et le reste de la grille est initialise a 0
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, 0, i) != TILE_4)
      return false;
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, 1, i) != TILE_2)
      return false;
  result = check_case_empty(g, 0, GRID_SIDE, 2, GRID_SIDE);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, END_GRID, i, TILE_2);
  do_move(g, LEFT);
  // On met des 2 sur les deux premieres lignes, et le reste de la grille est initialise a 0
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, 0, i) != TILE_4)
      return false;
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, 1, i) != TILE_4)
      return false;
  result = check_case_empty(g, 0, GRID_SIDE, 2, GRID_SIDE);
  
  do_move(g, LEFT);
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, 0, i) != TILE_8)
      return false;
  result = check_case_empty(g, 0, GRID_SIDE, 1, GRID_SIDE);
  return result;
}


bool test_do_move_right(grid g){
  bool result = true;
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, EMPTY_TILE);
  
   
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, END_GRID, i, TILE_2);
   
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, 0, i, TILE_2);
  do_move(g, RIGHT);
  // On met des 2 sur la premiere ligne, et le reste de la grille est initialise a 0
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, END_GRID, i) != TILE_4)
      return false;
  result = check_case_empty(g, 0, GRID_SIDE, 0, END_GRID);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, 0, i, TILE_2);
  do_move(g, RIGHT);
  // On met des 2 sur la premiere ligne, des 1 sur la deuxieme ligne et le reste de la grille est initialise a 0
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, END_GRID, i) != TILE_4)
      return false;
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, END_GRID - 1, i) != TILE_2)
      return false;
  result = check_case_empty(g, 0, GRID_SIDE, 0, GRID_SIDE - 2);
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, 0, i, TILE_2);
  do_move(g, RIGHT);
  // On met des 2 sur les deux premieres lignes, et le reste de la grille est initialise a 0
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, END_GRID, i) != TILE_4)
      return false;
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, END_GRID - 1, i) != TILE_4)
      return false;
  result = check_case_empty(g, 0, GRID_SIDE, 0, HALF_GRID_SIDE);
 
  do_move(g, RIGHT);
  for(int i = 0; i < GRID_SIDE; i++)
    if(get_tile(g, END_GRID, i) != TILE_8)
      return false;
  result = check_case_empty(g, 0, GRID_SIDE, 0, GRID_SIDE/4);
  return result;
}



bool test_do_move_all(grid g){
  bool result = true;
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, TILE_2); // On initialise toute la grille a 2
  
  do_move(g, UP);
  
  //cas d'une grille impaire
  if(GRID_SIDE%2 == 1){
    
    for(int i = 0; i < GRID_SIDE; i++){
      if(get_tile(g, i, GRID_SIDE/2) != TILE_2)
	return false;
      for(int j = 0; j < GRID_SIDE/2; j++)
	if(get_tile(g, i, j) != TILE_4)
	  return false;
    }
    
     do_move(g, DOWN);
     int nbLine8 = GRID_SIDE/4;
     for(int i = 0; i < GRID_SIDE; i++){
       if(get_tile(g, i, END_GRID) != TILE_2)
	 return false;
       if((HALF_GRID_SIDE+1)%2 == 0){
	 if(get_tile(g, i, END_GRID - nbLine8 - 1) != TILE_4)
	   return false;
       }
       for(int j = nbLine8; j > 0; j--)
	 if(get_tile(g, i, END_GRID - j) != TILE_8)
	   return false;
     }

     do_move(g, RIGHT);
     for(int i = HALF_GRID_SIDE+1; i < GRID_SIDE; i++){
       if(get_tile(g, i, END_GRID) != TILE_4)
	 return false;
       if((HALF_GRID_SIDE+1)%2 == 0)
	 if(get_tile(g, i, END_GRID - nbLine8 - 1) != TILE_8)
	   return false;
       
       for(int j = nbLine8; j > 0; j--){
	 if((HALF_GRID_SIDE+1)%2 == 1)
	   if(get_tile(g, HALF_GRID_SIDE, END_GRID - j) != TILE_8)
	     return false; 
	 if(get_tile(g, i, END_GRID - j) != TILE_16)
	   return false;
       }
     }
     if((HALF_GRID_SIDE+1)%2 == 0){
       if(get_tile(g, HALF_GRID_SIDE, END_GRID) != TILE_2)
	 return false;
       if(get_tile(g, HALF_GRID_SIDE, END_GRID - nbLine8 - 1) != TILE_4)
	 return false;
     }

     do_move(g, LEFT);
     for(int i = 1; i <= nbLine8 ; i++){
       if(get_tile(g, i, END_GRID) != TILE_8)
	 return false;
       if((HALF_GRID_SIDE+1)%2 == 0)
	 if(get_tile(g, i, END_GRID - nbLine8 - 1) != TILE_16)
	   return false;
       if(get_tile(g, i, END_GRID - i) != TILE_32)
	 return false;
       if(get_tile(g, 0, END_GRID - i) != TILE_8)
	 return false;
     }
     if(get_tile(g, 0, END_GRID) != TILE_2)
       return false;
              
     if((HALF_GRID_SIDE+1)%2 == 0){
       if(get_tile(g, 0, END_GRID - nbLine8 - 1) != TILE_4)
	  return false; 
       if(get_tile(g, 0 + nbLine8 + 1, END_GRID - nbLine8 - 1) != TILE_8)
	 return false;
       if(get_tile(g, 0 + nbLine8 + 1, END_GRID) != TILE_4)
	 return false;
       for(int i = 0; i < nbLine8; i++)
	 if(get_tile(g, 0 + nbLine8 + 1, END_GRID - i - 1) != TILE_16)
	   return false;
     }
  }
  else{
    for(int i = 0; i < GRID_SIDE; i++){
      for(int j = 0; j < HALF_GRID_SIDE; j++)
	if(get_tile(g, i, j) != TILE_4)
	  return false;
    }

    do_move(g, DOWN);
    for(int i = 0; i < GRID_SIDE; i++){
      if((HALF_GRID_SIDE)%2 == 1)
	if(get_tile(g, i, END_GRID - GRID_SIDE/4) != TILE_4)
	  return false;
	   
      for(int j = 0; j < GRID_SIDE/4; j++){
	if(get_tile(g, i, END_GRID - j) != TILE_8)
	  return false;
	
      }
    }

    do_move(g, RIGHT);
    for(int i = HALF_GRID_SIDE + 1; i < GRID_SIDE; i++){
      if((HALF_GRID_SIDE)%2 == 1)
	if(get_tile(g, i, END_GRID - GRID_SIDE/4) != TILE_8)
	  return false;
      for(int j = END_GRID; j > END_GRID - GRID_SIDE/4; j--)
	if(get_tile(g, i, j) != TILE_16)
	  return false;
    }

    do_move(g, LEFT);
    for(int i = 0; i < GRID_SIDE/4; i++){
      if((HALF_GRID_SIDE)%2 == 1)
	if(get_tile(g, i, END_GRID - GRID_SIDE/4) != TILE_16)
	  return false;
      for(int j = END_GRID; j > END_GRID - GRID_SIDE/4; j--){
	if((HALF_GRID_SIDE)%2 == 1)
	  if(get_tile(g, GRID_SIDE/4, j) != TILE_16)
	    return false;
	if(get_tile(g, i, j) != TILE_32)
	  return false;
      }
    }
    if((HALF_GRID_SIDE)%2 == 1){
      if(get_tile(g, GRID_SIDE/4, END_GRID - GRID_SIDE/4) != TILE_8)
	return false;
    }
  }
  return result;
}       

bool test_can_move(grid g){
  // On initialise toute la grille a 0
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, EMPTY_TILE); 
  if(can_move(g, UP) || can_move(g, DOWN) || can_move(g, RIGHT) || can_move(g, LEFT) )
    return false;

  // On place une tuile en plein milieu de la grille
  set_tile(g, HALF_GRID_SIDE, HALF_GRID_SIDE, TILE_2);
  if( !can_move(g, UP) || !can_move(g, DOWN) || !can_move(g, RIGHT) || !can_move(g, LEFT))
     return false;
  set_tile(g, HALF_GRID_SIDE, HALF_GRID_SIDE, EMPTY_TILE);

  // On rempli la ligne du  bas de fusion possible, avec un compteur qui creera des tuiles de valeurs croissantes
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, i, END_GRID, TILE_2);
  if(!can_move(g, UP) || can_move(g, DOWN) || !can_move(g,RIGHT) || !can_move(g, LEFT))
    return false;
  
  // On interdit les fusion
  int tile = 1;
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, i, END_GRID, tile++);
  if(!can_move(g, UP) || can_move(g, DOWN) || can_move(g,RIGHT) || can_move(g, LEFT))
    return false;
  
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, i, END_GRID, EMPTY_TILE);
  
  // On place deux tuiles en haut a gauche de la grille
  set_tile(g, 0, 0, TILE_2);
  set_tile(g, 1, 0, TILE_2);
  if(can_move(g, UP) || !can_move(g, DOWN) || !can_move(g, RIGHT) || !can_move(g, LEFT))
    return false;
  set_tile(g, 1, 0, EMPTY_TILE);
  
  // On place une deuxieme tuiles sous la premiere (en x = 0; fusion possible)
  set_tile(g, 0, 1, TILE_2);
  if(!can_move(g, UP) || !can_move(g, DOWN) || !can_move(g, RIGHT) || can_move(g, LEFT))
    return false;
  
  // On la change (fusion impossible)
  set_tile(g, 0, 1, TILE_4);
  if(can_move(g, UP) || !can_move(g, DOWN) || !can_move(g, RIGHT) || can_move(g, LEFT))
    return false;
  // On continue de remplir la colonne de tiles a valeur croissantes(fusion impossible)
  tile = 1;
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, 0, i, tile++);
  if(can_move(g, UP) || can_move(g, DOWN) || !can_move(g, RIGHT) || can_move(g, LEFT))
    return false;
  
  // On autorise une fusion
  for(int i = 0; i < 2; i++)
    set_tile(g, 0, i,TILE_2);
  
  if(!can_move(g, UP) || !can_move(g, DOWN) || !can_move(g, RIGHT) || can_move(g, LEFT))
    return false;
  
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, EMPTY_TILE); // On initialise toute la grille a 0
  if(can_move(g, UP) || can_move(g, DOWN) || can_move(g, RIGHT) || can_move(g, LEFT))
    return false;
  // On remplit la derniere colonne du tableau
  for(int i = 0; i < GRID_SIDE; i++)
    set_tile(g, END_GRID, i, TILE_2);
  if(!can_move(g, UP) || !can_move(g, DOWN) || !can_move(g, LEFT) || can_move(g, RIGHT))
     return false;

   return true;  
}


bool test_add_tile(grid g){
  int rand4 = 0;
  for(int i = 0; i < GRID_SIDE; i++)
    for(int j = 0; j < GRID_SIDE; j++)
      set_tile(g, i, j, EMPTY_TILE); // On initialise toute la grille a 0
  for(int k = GRID_SIDE * GRID_SIDE; k > 0; k--)
    add_tile(g);
  for(int i = 0; i <  GRID_SIDE; i++)
    for(int j = 0; j <  GRID_SIDE; j++){
      if(get_tile(g, i, j) == EMPTY_TILE)
	return false;
      if
	(get_tile(g, i, j) == TILE_4)
	rand4 += 1;
    }
  if(rand4 < 1)
    return false;
  return true;
}


