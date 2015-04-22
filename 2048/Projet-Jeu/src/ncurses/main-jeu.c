#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <grid.h>
#include <ncurses.h>
#include <curses.h>
#include <time.h>
#define PATH_HIGHSCORE "../src/ncurses/highscore_ncurses.txt"
#define MARGIN_LEFT 3
#define MARGIN_TOP 3
#define HEIGHT_CASE 4
#define WIDTH_CASE 8
#define NUMBER_COLOR 8
#define BELOW_CASE 8
#define RIGHT_CASE 4
#define X_DISPLAY_GAME_OVER 9
#define Y_DISPLAY_GAME_OVER 11*GRID_SIDE+1
#define X_DISPLAY_REPLAY 11
#define Y_DISPLAY_REPLAY 9*GRID_SIDE+1
#define X_DISPLAY_2048 1
#define Y_DISPLAY_2048 4*GRID_SIDE-1
#define X_DISPLAY_SCORE 5
#define Y_DISPLAY_SCORE 9*GRID_SIDE+1
#define X_DISPLAY_HIGHSCORE 6
#define Y_DISPLAY_HIGHSCORE 9*GRID_SIDE+1
#define X_RETRY_QUIT_GAME 4*GRID_SIDE+5
#define Y_RETRY_QUIT_GAME 3

#define KEY_Y 110
#define KEY_Q 113
#define KEY_N 121
#define KEY_R 114
/**
 * \file main-jeu.c
 * \brief fichier permetant le jeu en ncurses
 **/
 

/**
 * \brief affiche la grille
 * \param g grille
 * \return 
 */
static void display_grid(grid g);

/**
 * \brief affiche le game_over
 * \param *continue_game,*valid_answer pointeur sur un bool
 * \return 
 */
static void display_game_over(bool *continue_game, int *valid_answer);

/**
 * \brief recupere le highscore
 * \return unsigned long int
 */
static unsigned long int read_highscore();

/**
 * \brief ecrit le highscore
 * \param score le score a ecrire
 * \return
 */
static void write_highscore(unsigned long int score);


int main(int argc, char *argv[]){
  keypad(stdscr, TRUE);
  bool continue_game = true;
  srand(time(NULL));
  while(continue_game){
    grid g = new_grid();
    add_tile(g);
    add_tile(g);
    int ch = 0;
    display_grid(g);
    int valid_answer = 0;
    while(!game_over(g) && valid_answer == 0){
      ch=getch();
      switch(ch){
      case KEY_UP:
	if (can_move(g, UP))
	  play(g, UP);
	break;
      case KEY_DOWN:
	if (can_move(g, DOWN))
	  play(g, DOWN);
	break;
      case KEY_RIGHT:
	if (can_move(g, RIGHT))
	  play(g, RIGHT);
	break;
      case KEY_LEFT:
	if (can_move(g, LEFT))
	  play(g, LEFT);
	break;
      case KEY_Q: // "q" pour quitter
	continue_game = false;
	valid_answer = 1;
	break;
      case KEY_R: // "r" pour rejouer
	valid_answer = 1;
	break;
      }
      display_grid(g);
    }
    while(valid_answer == 0 && continue_game == true)
      display_game_over(&continue_game, &valid_answer);
    delete_grid(g);
    endwin();    
  }
  return EXIT_SUCCESS;
}


static void display_game_over(bool *continue_game, int *valid_answer){
  keypad(stdscr, TRUE);
  int ch = 0;
  mvprintw(X_DISPLAY_GAME_OVER, Y_DISPLAY_GAME_OVER , "GAME OVER");
  mvprintw(X_DISPLAY_REPLAY, Y_DISPLAY_REPLAY, "Voulez-vous rejouer? y or n ? ");
  refresh(); 
  cbreak();   
  ch = getch();
  if(ch == KEY_Y){
    *continue_game = false; // Arrete le programme et sort de la boucle
    *valid_answer = 1;
  }
  if(ch == KEY_N)
    *valid_answer = 1; // Relance le programme du debut 
  endwin();    
}  

static void display_grid(grid g){
  initscr();
  clear();
  keypad(stdscr, TRUE);
  // Mise en forme de la grille
  int x = MARGIN_TOP;
  int y = MARGIN_LEFT-1;
  for(int i = 0; i <= GRID_SIDE; i++){
    // tracer des lignes horizontales
    mvhline(x, MARGIN_LEFT, ACS_HLINE, 8*GRID_SIDE-1);
    x += HEIGHT_CASE;
  }
  for(int i = 0; i <= GRID_SIDE; i++){
    //tracer des lignes verticales
    mvvline(MARGIN_TOP+1, y, ACS_VLINE, 4*GRID_SIDE-1);
    y += WIDTH_CASE;
  }
  start_color();
  // Definition des couleurs (rouge, vert, jaune, bleu fonce, violet, bleu turquoise, blanc)
  for(int i = 1; i < NUMBER_COLOR; i++){
    init_pair(i, i, 0);
  }
  // Mise des valeurs dans la grille
  x = 5; 
  int t[] = {COLOR_RED, COLOR_YELLOW, COLOR_GREEN, COLOR_CYAN, COLOR_BLUE, COLOR_MAGENTA, COLOR_WHITE};
  for(int i = 0; i < GRID_SIDE; i++){
    y = 5;
    for(int j = 0; j < GRID_SIDE; j++){
      if(get_tile(g, j, i) != 0){
	// coloration active
	attron(COLOR_PAIR(t[(get_tile(g, j, i) - 1) % (NUMBER_COLOR-1)]));
	// Recuperation d'un nombre et de la coloration
	char buff[5];
	sprintf(buff, "%d", (unsigned int)pow(2, get_tile(g, j, i)));
	mvprintw(x, y, buff);
	// coloration desactive
	attroff(COLOR_PAIR(1));
      }
      y += BELOW_CASE;
    }
    x += RIGHT_CASE;
  }
  mvprintw(X_DISPLAY_2048, Y_DISPLAY_2048, "2048");
  mvprintw(X_DISPLAY_SCORE, Y_DISPLAY_SCORE, "Score: %lu ", grid_score(g));
  unsigned long int read_highscore();
  if(grid_score(g) > read_highscore()){
    write_highscore(grid_score(g));
  }
  mvprintw(X_DISPLAY_HIGHSCORE, Y_DISPLAY_HIGHSCORE, "High Score: %lu ", read_highscore());
  mvprintw(X_RETRY_QUIT_GAME, Y_RETRY_QUIT_GAME , "Recommencer Partie / Quitter Partie (r / q)");
  refresh();
}

static unsigned long int read_highscore(){
  unsigned long int score[1];
  FILE *highscore = NULL;
  highscore = fopen(PATH_HIGHSCORE,"r");
  if(highscore == NULL)
    return 0;
  else{
    fscanf(highscore, "%lu", &score[0]);
    fclose(highscore);
  }
  return score[0];
}

static void write_highscore(unsigned long int score){
  FILE *highscore = NULL;
  highscore = fopen(PATH_HIGHSCORE,"w");
  fprintf(highscore, "%lu", score);
  fclose(highscore);
}
