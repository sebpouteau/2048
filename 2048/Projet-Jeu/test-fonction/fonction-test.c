#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "../src/grid.h"
#include "../src/gridIA.h"
#include "fonction-test.h"

bool test_delete_grid(){
  bool resultat = true;
  grid g = new_grid();
  delete_grid(g);
  if (g !=NULL)
    resultat = false;
  return resultat;
}
  
bool test_new_grid(){
  bool resultat = true;
  grid g = new_grid();
  if (g==NULL)
    resultat=false;
  delete_grid(g);
  return resultat;
}


bool test_get_tile(grid g){
  bool resultat = true;
  for (int i = 0 ; i<GRID_SIDE;i++)
    for (int j = 0 ; j<GRID_SIDE;j++)
      if (get_tile(g,i,j)!=0)
 	resultat = false;
  
  /* if (get_tile(g,GRID_SIDE,GRID_SIDE)!=-1){ */
  /*   resultat = false; */
  //}
  
  return resultat;
}

bool test_set_tile(grid g){
  bool resultat = true;
  //passage de toute les cases à 2
  for (int i = 0 ; i<GRID_SIDE;i++)
    for (int j = 0 ; j<GRID_SIDE;j++)
      set_tile(g,i,j,2);
  //verification que les cases sont à 2
  for (int i = 0 ; i<GRID_SIDE;i++)
    for (int j = 0 ; j<GRID_SIDE;j++)
      if (get_tile(g,i,j)!=2)
 	resultat = false;
  //reset de la grid a zero
  for (int i = 0 ; i<GRID_SIDE;i++)
    for (int j = 0 ; j<GRID_SIDE;j++)
      set_tile(g,i,j,0);

  return resultat;
}

bool test_copy_grid(grid g){
  bool resultat = true;
  grid g1 = new_grid();
  for (int i = 0 ; i<GRID_SIDE;i++)
    for (int j = 0 ; j<GRID_SIDE;j++)
      if (get_tile(g,i,j)!= get_tile(g1,i,j))
 	resultat = false;
    copy_grid(g,g1);
  return resultat;;
}
bool test_get_score_grid(grid g){
  bool resultat = true;
  if (grid_score(g)!=0)
    resultat = false;
  return resultat;
}

bool test_set_score_grid(grid g){
  bool resultat = true;
  set_grid_score(g,20);
  if (grid_score(g)!=20)
    resultat = false;
  set_grid_score(g,-1);
  if (grid_score(g)==0)
    resultat = false;
  return resultat;
}

bool test_game_over(grid g){
  bool resultat = true;
  int nbr=1;
  for (int i = 0 ; i<GRID_SIDE;i++)
    for (int j = 0 ; j<GRID_SIDE;j++){
      set_tile(g,i,j,nbr);
      nbr++;
    }
  if (!game_over(g))
    resultat = false;
  for (int i = 0 ; i<GRID_SIDE;i++)
    for (int j = 0 ; j<GRID_SIDE;j++)
      set_tile(g,i,j,2);
  if (game_over(g))
    resultat = false;
  for (int i = 0 ; i<GRID_SIDE;i++)
    for (int j = 0 ; j<GRID_SIDE;j++)
      set_tile(g,i,j,0);
  set_tile(g,0,0,2);
  if (game_over(g))
    resultat = false;
  return resultat;
}

 bool test_do_move_up(grid g){
  bool resultat = true;
  for (int i = 0 ; i<GRID_SIDE;i++)
    for (int j = 0 ; j<GRID_SIDE;j++)
      set_tile(g,i,j,0);
  for (int i =0 ; i<GRID_SIDE;i++){
    set_tile(g,0,i,2);
    do_move(g,UP);
    if ( get_tile(g,0,i)!=2)
      resultat = false;
    set_tile(g,GRID_SIDE-1,i,2);
    do_move(g,UP);
    if ( get_tile(g,0,i)!=4)
      resultat = false;
    set_tile(g,GRID_SIDE-1,i,2);
    do_move(g,UP);
    if ( get_tile(g,0,i)!=4 && get_tile(g,1,i)!=2)
      resultat = false;
    set_tile(g,GRID_SIDE-1,0,2);
    do_move(g,UP);
  }  

  return resultat;
}

bool test_can_move_up(grid g){
  bool resultat = true;
  for (int i = 0 ; i<GRID_SIDE;i++)
    for (int j = 0 ; j<GRID_SIDE;j++)
      set_tile(g,i,j,0); // on initialise tout à 0
  if(can_move(g, UP))
    return false;

  set_tile(g, 3, 0, 2);
  set_tile(g, 3, 1, 2);
  if(!can_move(g, UP))
    return false;
  set_tile(g, 3, 0, 0);
  set_tile(g, 3, 1, 0);
  
  set_tile(g, 0, 0, 2);
  set_tile(g, 0, 1, 2);
  if(can_move(g, UP))
    return false;
  set_tile(g, 2, 2, 2);
  if(!can_move(g, UP))
    return false;
  set_tile(g, 0, 0, 0);
  set_tile(g, 2, 2, 0);

  set_tile(g, 1, 1, 2);
  if(!can_move(g, UP))
    return false;
  
  set_tile(g, 1, 1, 4);
   if(can_move(g, UP))
    return false;

   set_tile(g, 2, 1, 2);
   set_tile(g, 3, 1, 4);
   if(can_move(g, UP))
    return false;
   set_tile(g, 3, 1, 2);
   
   if(!can_move(g, UP))
    return false;

   for(int i = 0; i< GRID_SIDE - 1; i++)
     set_tile(g, i, 1, 0);
    if(!can_move(g, UP))
      return false;
  
  return resultat;
}



bool test_can_move_down(grid g){

  bool resultat = true;
  for (int i = 0 ; i<GRID_SIDE;i++)
    for (int j = 0 ; j<GRID_SIDE;j++)
      set_tile(g,i,j,0); // on initialise tout à 0
  if(can_move(g, DOWN))
    return false;

  set_tile(g, 0, 0, 2);
  set_tile(g, 0, 1, 2); //on pose des tiles en (0,0) et (0,1)
  if(!can_move(g,DOWN))
    return false;
  set_tile(g, 0, 0, 0);
  set_tile(g, 0, 1, 0);
   
  set_tile(g, 3, 0, 2);//on pose des tiles en (3,0) et (3,1)
  set_tile(g, 3, 1, 2);
  if(can_move(g, DOWN))
    return false;
  set_tile(g, 2, 2, 2);
  if(!can_move(g, DOWN))
    return false;
  set_tile(g, 2, 2, 0);
  set_tile(g, 3, 0, 0);

  set_tile(g, 2, 1, 2);
  if(!can_move(g, DOWN))
    return false;
  
  set_tile(g, 2, 1, 4);
   if(can_move(g, DOWN))
    return false;

   set_tile(g, 1, 1, 2);
   set_tile(g, 0, 1, 4);
   if(can_move(g, DOWN))
    return false;
   set_tile(g, 0, 1, 2);
   
   if(!can_move(g, DOWN))
    return false;

   for(int i = 3; i>0; i--)
     set_tile(g, i, 1, 0);
    if(!can_move(g, DOWN))
      return false;
  
  return resultat;
}





bool test_can_move_left(grid g){

  bool resultat = true;
  for (int i = 0 ; i<GRID_SIDE;i++)
    for (int j = 0 ; j<GRID_SIDE;j++)
      set_tile(g,i,j,0); // on initialise tout à 0
   if(can_move(g, LEFT))
    return false;

   set_tile(g, 0, 0, 2);
   set_tile(g, 1, 0, 2);
   if(can_move(g, LEFT))
     return false;
   set_tile(g, 2, 2, 2);
   if(!can_move(g, LEFT))
     return false;
   set_tile(g, 2, 2, 0);
   
   set_tile(g, 0, 1, 2);
   if(!can_move(g,LEFT))
     return false;
   
   set_tile(g, 0, 1, 4);
   if(can_move(g, LEFT))
     return false;
   
   set_tile(g, 0, 2, 2);
   set_tile(g, 0, 3, 4);
   if(can_move(g, LEFT))
     return false;
   
   set_tile(g, 0, 3, 2);
   if(!can_move(g, LEFT))
     return false;
   
   for(int j = 0; j>GRID_SIDE -1; j++)
     set_tile(g, 0, j, 0);
   if(!can_move(g, LEFT))
     return false;
   
  return resultat;
}





bool test_can_move_right(grid g){
  bool resultat = true;
   for (int i = 0 ; i<GRID_SIDE;i++)
    for (int j = 0 ; j<GRID_SIDE;j++)
      set_tile(g,i,j,0); // on initialise tout à 0
   if(can_move(g, RIGHT))
    return false;

   set_tile(g, 0, 3, 2);
   set_tile(g, 1, 3, 2);
   if(can_move(g, RIGHT))
     return false;
   set_tile(g, 2, 2, 2);
   if(!can_move(g, RIGHT))
     return false;
   set_tile(g, 2, 2, 0);
   
   set_tile(g, 0, 2, 2);
   if(!can_move(g,RIGHT))
     return false;
   
   set_tile(g, 0, 2, 4);
   if(can_move(g, RIGHT))
     return false;
   
   set_tile(g, 0, 1, 2);
   set_tile(g, 0, 0, 4);
   if(can_move(g, RIGHT))
     return false;
   
   set_tile(g, 0, 0, 2);
   if(!can_move(g, RIGHT))
     return false;
   
   for(int j = 3; j>0; j--)
     set_tile(g, 0, j, 0);
   if(!can_move(g, RIGHT))
     return false; 

  return resultat;
}






bool test_play(grid g){
  bool resultat = true;
  return resultat;
}

bool test_add_tile(grid g){
  bool resultat = true;
  int rand4 =0;
  for (int i = 0 ; i<GRID_SIDE;i++)
    for (int j = 0 ; j<GRID_SIDE;j++)
      set_tile(g,i,j,0); // on initialise tout à 0
  for(int c = 16; c >0; c--)
    add_tile(g);
  for(int i = 0; i< GRID_SIDE; i++)
    for(int j = 0; j< GRID_SIDE; j++){
      if(get_tile(g, i, j) == 0)
	return false;
      if(get_tile(g, i, j) == 4)
	rand4 +=1;
    }
  if(rand4<1)
    return false;
  return resultat;
}

/* 
  //test-set_tile/get_tile;
  for(int i = 0;i<4;i++)
    set_tile(g,i,0,2);
  for(int i = 0;i<4;i++){
    if(get_tile(g,i,0)!=2){
      resultat=false;
      break;
    }
  }
  printf(resultat?"Test 1/7 - set_tile :  Ok":"Test 1/6 - set_tile : False");
  printf("\n");
  
  
  //test copy_grid;
  grid g1 = new_grid(grid g);
  copy_grid(g,g1);
  printf( egalite_grid(g,g1) ? "Test 2/7 - copy_grid : Ok" : "Test 2/6 - copy_grid : False");
  printf("\n");
  
  resultat = true;
  // test get_grid_score/set_grid_score;
  do_move(g,d);
  if(grid_score(g) != 8)
    resultat=false;
  printf(resultat?"Test 3/7 - grid_score : Ok":"Test 3/6 - grid_score : False");
  printf("\n");
  
  resultat = true;
  //test Game_Over;
  int compt = 1;
  for(int i = 0;i<GRID_SIDE;i++){
    for(int j = 0;j<GRID_SIDE;j++){
      set_tile(g,i,j,compt);
      compt++;
    }
  }
  if(!game_over(g)) // si il indique que ce n'est pas gameover c'est que ne fonctionne pas
    resultat=false;
  set_tile (g,0,1,1);
  if (game_over(g))  //si il indique que c'est game_over c'est qu'il ne fonctionne pas
    resultat=false;
  printf(resultat ? "Test 4/7 - game_over : Ok" : "Test 4/6 - Game_Over : False");
  printf("\n");
  
  resultat = true;
  for(int i = 0;i<GRID_SIDE;i++)
    for(int j = 0;j<GRID_SIDE;j++)
      set_tile(g,i,j,0);
  set_tile(g,3,0,2);
  do_move(g,d);
  printf(get_tile(g,0,0)==2?"Test 5/7 - do_move d : Ok\n":"Test 5/6 - do_move : False\n");
  do_move(g,DOWN);
  printf(get_tile(g,3,0)==2?"         - do_move DOWN : Ok\n":"Test 5/6 - do_move : False\n");
  do_move(g,RIGHT);
  printf(get_tile(g,3,3)==2?"         - do_move RIGHT : Ok\n":"Test 5/6 - do_move : False\n");
  do_move(g,LEFT);
  printf(get_tile(g,3,0)==2?"         - do_move LEFT : Ok\n":"Test 5/6 - do_move : False\n");
  for(int i = 0;i<GRID_SIDE;i++)
    for(int j = 0;j<GRID_SIDE;j++)
      set_tile(g,i,j,2);
  do_move(g,d);
  do_move(g,DOWN);
  do_move(g,LEFT);
  do_move(g,RIGHT);

  if (get_tile(g,3,3)!=32)
    resultat=false;
  set_tile(g,3,0,32);
  do_move(g,LEFT);
  if (get_tile(g,3,0)!=64)
    resultat=false;
  set_tile(g,3,2,64);
  do_move(g,LEFT);
  if (get_tile(g,3,0)!=128)
    resultat=false;
  printf(resultat ?"Test 6/7 - do_move avec fusion: Ok":"Test 5/6 - do_move : False");

  printf("\n");
  
  //test delete_grid;
  delete_grid(g);
  delete_grid(g1);
  if(g == NULL || g1 == NULL)
    resultat =;
  printf(resultat ?"Test 7/7 - delete_Grid : Ok":"Test 6/6 - delete_Grid : False");
  printf("\n");
  
  resultat = true;
*/
