#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <grid.h>
#include <ncurses.h>
#include <curses.h>
#include <time.h>

#define MARGIN_LEFT 3
#define MARGIN_TOP 3
#define HEIGHT_CASE 4
#define WIDTH_CASE 8
#define NUMBER_COLOR 8
#define BELOW_CASE 8
#define RIGHT_CASE 4

#define KEY_Y 110
#define KEY_Q 113
#define KEY_N 121
#define KEY_R 114

static void display_grid(grid g);
static void display_gameOver(bool *continue_game, int *valid_answer);
static unsigned long int read_highscore();
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
      display_gameOver(&continue_game, &valid_answer);
    delete_grid(g);
    endwin();    
  }
  return EXIT_SUCCESS;
}


static void display_gameOver(bool *continue_game, int *valid_answer){
  keypad(stdscr, TRUE);
  int ch = 0;
  mvprintw(9, 11*GRID_SIDE+1, "GAME OVER");
  mvprintw(11, 9*GRID_SIDE+1, "Voulez-vous rejouer? y or n ? ");
  refresh(); 
  cbreak();   
  ch = getch();
  // 110 correspond à "y"
  if(ch == KEY_Y){
    *continue_game = false; // Arrete le programme et sort de la boucle
    *valid_answer = 1;
  }
  // 121 correspond à "n"
  if(ch == KEY_N)
    *valid_answer = 1; // Relance le programme du début 
  endwin();    
}  

static void display_grid(grid g){
  initscr();
  clear();
  keypad(stdscr, TRUE);
  // Mise en forme de la grille
  int x = MARGIN_TOP;
  int y = MARGIN_LEFT-1;
  for(int i = 0; i < GRID_SIDE+1; i++){
    // tracer des lignes horizontales
    mvhline(x, MARGIN_LEFT, ACS_HLINE, 8*GRID_SIDE-1);
    x += HEIGHT_CASE;
  }
  for(int i = 0; i < GRID_SIDE+1; i++){
    //tracer des lignes verticales
    mvvline(MARGIN_TOP+1, y, ACS_VLINE, 4*GRID_SIDE-1);
    y += WIDTH_CASE;
  }
  start_color();
  // Définition des couleurs (rouge, vert, jaune, bleu foncé, violet, bleu turquoise, blanc)
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
	// coloration activé
	attron(COLOR_PAIR(t[(get_tile(g, j, i) - 1) % (NUMBER_COLOR-1)]));
	// Récupération d'un nombre et de la coloration
	char buff[5];
	sprintf(buff, "%d", (unsigned int)pow(2, get_tile(g, j, i)));
	mvprintw(x, y, buff);
	// coloration desactivé
	attroff(COLOR_PAIR(1));
      }
      y += BELOW_CASE;
    }
    x += RIGHT_CASE;
  }
  mvprintw(1, 4*GRID_SIDE-1 , "2048");
  mvprintw(5, 9*GRID_SIDE+1, "Score: %lu ", grid_score(g));
  unsigned long int read_highscore();
  if(grid_score(g) > read_highscore()){
    write_highscore(grid_score(g));
  }
  mvprintw(6, 9*GRID_SIDE+1, "High Score: %lu ", read_highscore());
  mvprintw(4*GRID_SIDE+5, MARGIN_LEFT, "Recommencer Partie / Quitter Partie (r / q)");
  refresh();
}

static unsigned long int read_highscore(){
  unsigned long int score[1];
  FILE *highscore = NULL;
  highscore = fopen("../ncurses/highscore_ncurses.txt","r");
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
  highscore = fopen("../ncurses/highscore_ncurses.txt", "w");
  fprintf(highscore, "%lu", score);
  fclose(highscore);
}
