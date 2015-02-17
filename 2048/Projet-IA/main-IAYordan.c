#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "../Projet-Jeu/grid.h"
#include <ncurses.h>
#include <curses.h>
#include <time.h>

static void display_grid(grid g,int *ch);
/* static void display_gameOver(bool *continuer,int *reponse_valide); */
static bool objectif_atteint(grid g);
static long maximum_tile(grid g);

int main(int argc,char **argv){

  grid g= new_grid();
  int ch=0;
  srand(time(NULL));
  while(!game_over(g)){
    while( can_move(g,UP) ||  can_move(g, LEFT)){
      play(g, LEFT);
      play(g,UP);
     
    }
    if(can_move(g,UP)&& !can_move(g,LEFT))
      play(g,UP);
     
    if(!can_move(g, UP) && !can_move(g,LEFT)){
      play(g,RIGHT);
      play(g,LEFT);
     
   
    }
    if(!can_move(g, UP) && !can_move(g,LEFT) && !can_move(g, RIGHT)){
      play(g,DOWN);
      play(g, UP);
    }
    

  }
  display_grid(g,&ch);
  printf("Objectif atteint? %s \n", objectif_atteint(g)?"oui":"non");
  printf("Tile max = %ld \n", maximum_tile(g));
}



static bool objectif_atteint(grid g){
  for(int i = 0; i<GRID_SIDE; i++){
    for(int j = 0; j< GRID_SIDE; j++){
      if(get_tile(g, i, j)>=2048)
	return true;
    }
  }
  return false;
}

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
    
static void display_grid(grid g,int *ch){
  initscr();
  clear();
  newwin(0,0,40,40);
  keypad(stdscr, TRUE);
  // mise en forme de la grille
  int x=2;
  int y=1;
  for (int i=0;i<5;i++){
    mvhline(x,2,0,31);
    x+=4;
    mvvline(3,y,0,15);
    y+=8;
  }
  start_color();
  init_pair(1,1,0); //rouge
  init_pair(2,2,0); //vert
  init_pair(3,3,0); //jaune
  init_pair(4,4,0); //bleu fonce
  init_pair(5,5,0); //violet
  init_pair(6,6,0); // bleu turquoise
  init_pair(7,7,0); //blanc

  // mise des valeur dans la grille
  x=4; 
  for (int i=0;i<GRID_SIDE;i++){
    y=4;
    for (int j=0;j<GRID_SIDE;j++){
      if (get_tile(g,i,j)!=0){
	  switch(get_tile(g,i,j)){
	  case 2:
	    attron(COLOR_PAIR(1));
	    break;
	  case 4:
	    attron(COLOR_PAIR(3));
	    break;
	  case 8:
	    attron(COLOR_PAIR(2));
	    break;
	  case 16:
	    attron(COLOR_PAIR(6));
	    break;
	  case 32:
	    attron(COLOR_PAIR(4));
	    break;
	  case 64:
	    attron(COLOR_PAIR(5));
	    break;
	  case 128:
	    attron(COLOR_PAIR(7));
	    break;
	  case 256:
	    attron(COLOR_PAIR(1));
	    break;
	  case 512:
	    attron(COLOR_PAIR(3));
	    break;
	  case 1024:
	    attron(COLOR_PAIR(2));
	    break;
	  default:
	    attron(COLOR_PAIR(6));
	    break;
	  }
	  //recuperation d'un nombre et coloration
	  char buff[5];
	  sprintf(buff, "%d", get_tile(g,i,j));
	  mvprintw(x,y,buff);
	  attroff(COLOR_PAIR(1));
      }
      y+=8;
    }
    x+=4;
  }
  mvprintw(1,15,"2048");
  char score[10];
  sprintf(score, "%lu", grid_score(g));
  mvprintw(19,5,"Score: ");
  mvprintw(19,12,score);
  refresh();
  cbreak();   
  *ch=getch();
  endwin();    
}
