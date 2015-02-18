#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "grid.h"
#include <ncurses.h>
#include <curses.h>
#include <time.h>

static void display_grid(grid g,int *ch);
static void display_gameOver(bool *continuer,int *reponse_valide);

int main(int argc,char **argv){

  bool continuer=true;
  srand(time(NULL));
  while (continuer){
    grid g = new_grid();
    int ch=0;
    display_grid(g,&ch);
    while(!game_over(g)){
      switch(ch){
      case KEY_UP:
	play(g,UP);
	break;
      case KEY_DOWN:
	play(g,DOWN);
	break;
      case KEY_RIGHT:
	play(g,RIGHT);
	break;
      case KEY_LEFT:
	play(g,LEFT);
	break;
      }
      display_grid(g,&ch);
    }
    int reponse_valide = 0;
    while (reponse_valide == 0)
      display_gameOver(&continuer,&reponse_valide);
  }
  return EXIT_SUCCESS;
}



static void display_gameOver(bool *continuer,int *reponse_valide){
  keypad(stdscr, TRUE);
  int ch=0;  
  mvprintw(21,5,"GAME OVER");
  mvprintw(23,2,"Voulez-vous rejouer? y or n ? ");
  refresh(); 
  cbreak();   
  ch=getch();
  if(ch==110){
    *continuer=false;//arrete le programme, sort de la boucle
    *reponse_valide = 1;
  }
  if(ch==121)
    *reponse_valide = 1;//relance le programme du debut 
  endwin();    
}  

   
 
static void display_grid(grid g,int *ch){
  initscr();
  clear();
  //newwin(0,0,40,40);
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
