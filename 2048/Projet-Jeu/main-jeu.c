#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "grid.h"
#include <ncurses.h>
#include <curses.h>
#include <time.h>

static void display_grid(grid g);
static void display_gameOver(bool *continuer,int *reponse_valide);
static unsigned long int read_highscore();
static void write_highscore(unsigned long int score);

int main(int argc,char **argv){
  keypad(stdscr, TRUE);
  bool continuer=true;
  srand(time(NULL));
  while (continuer){
    grid g = new_grid();
    int ch=0;
    display_grid(g);
    int reponse_valide = 0;
    while(!game_over(g) && reponse_valide==0){
      ch=getch();
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

      case 113: // quit avec q
	continuer=false;
	reponse_valide=1;
	break;
     case 114:
	reponse_valide=1;
	break;
      }
      display_grid(g);
    }
    
    while (reponse_valide == 0 && continuer==true)
      display_gameOver(&continuer,&reponse_valide);
    endwin();    
  }
  return EXIT_SUCCESS;
}



static void display_gameOver(bool *continuer,int *reponse_valide){
  keypad(stdscr, TRUE);
  int ch=0;  
  mvprintw(9,45,"GAME OVER");
  mvprintw(11,36,"Voulez-vous rejouer? y or n ? ");
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

   
 
static void display_grid(grid g){
  initscr();
  clear();
  keypad(stdscr, TRUE);
  // mise en forme de la grille
  int x=3;
  int y=2;
  for (int i=0;i<5;i++){
    mvhline(x,3,ACS_HLINE,31);
    x+=4;
  }
  for(int i=0; i<5; i++){
    mvvline(4,y,ACS_VLINE,15);
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
  x=5; 
  for (int i=0;i<GRID_SIDE;i++){
    y=5;
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
  mvprintw(5,36,"Score: %lu ",grid_score(g));
  unsigned long int read_highscore();
  if (grid_score(g)>read_highscore()){
    write_highscore(grid_score(g));
  }
  mvprintw(6,36,"High Score: %lu ",read_highscore());
  mvprintw(21,3,"Recommencer Partie / Quitter Partie (r / q)");

  refresh();
}
static unsigned long int read_highscore(){

  unsigned long int score[1];
  FILE *highscore = NULL; //On initialise un pointeur de fichier

  highscore = fopen("Highscore.txt","r"); //On ouvre HIGHSCORE.txt
  // "r" = read only

  if (highscore == NULL) //On a pas réussi a ouvrir le fichier
    return 0;

  else
    {
      fscanf(highscore,"%lu",&score[0]);
      fclose(highscore);
    }

  return score[0];
}


static void write_highscore(unsigned long int score){
  FILE *highscore = NULL;
  highscore=fopen("Highscore.txt","w"); //On ouvre HIGHSCORE.txt
  // "w" = write only => On va écraser le contenu du fichier
  fprintf(highscore,"%lu",score);
  fclose(highscore);
}
