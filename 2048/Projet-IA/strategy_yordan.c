#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
//#include "../Projet-Jeu/src/grid.h"
#include <time.h>
#include "strategy.h"
#include <ncurses.h>

typedef struct tabBestm* tabBestM;
struct tabBestm{
  float score;
  dir best; 
};

#define MONO_WEIGHT 47
#define MONO_POW 4
#define SMOOTH_WEIGHT 40
#define EMPTY_WEIGHT 270
#define MERGES_WEIGHT 700
#define LOST_PENALTY 20000
#define SUM_WEIGHT 3.5
#define SUM_POW 11



//static void display_grid(grid g);
static long int maximum_tile(grid g);
static tabBestM new_tabBestM();
static dir best_move(grid g);
static int empty_tiles(grid g);
static float monotonicity(grid g);
static long int smoothness(grid g);
static void fill_tabG(int empty, grid g, grid* tab);
static float eval_tabG(int empty,grid* tab);
static float nb_merges_and_empty(grid g);
static float eval_branch(grid g);

int main(int argc,char **argv){
  
  int cpt_8 = 0;
  int cpt_16 = 0;
  int cpt_32 = 0;
  int cpt_64 = 0;
  int cpt_128 = 0;
  int cpt_256 = 0;
  int cpt_512 = 0;
  int cpt_1024 = 0;
  int cpt_2048 = 0;
  int cpt_4096 = 0;
  
  srand(time(NULL));
  for (int j=0; j < 1;j++){
    grid g= new_grid();
    add_tile(g);
    add_tile(g);
    while(!game_over(g)){
      dir b = best_move(g);
      if(!can_move(g,b))
	continue;
      play(g,b);
    }
    if(maximum_tile(g) == 3)
      cpt_8 += 1;
    if(maximum_tile(g) == 4)
      cpt_16 += 1;
    if(maximum_tile(g) == 5)
      cpt_32 += 1;
    if(maximum_tile(g) == 6)
      cpt_64 += 1;
    if(maximum_tile(g) == 7)
      cpt_128 += 1;
    if(maximum_tile(g) == 8)
      cpt_256 += 1;
    if(maximum_tile(g) == 9)
      cpt_512 += 1;
    if(maximum_tile(g) == 10)
      cpt_1024 += 1;
    if(maximum_tile(g) == 11)
      cpt_2048 += 1;
    if(maximum_tile(g) == 12)
      cpt_4096 += 1;
    
    //delete_grid(g);
  }
  printf("\n --------------- \n");
  printf("Nombre de fois 8 : %d\n", cpt_8);
  printf("Nombre de fois 16 : %d\n", cpt_16);
  printf("Nombre de fois 32 : %d\n", cpt_32);
  printf("Nombre de fois 64 : %d\n", cpt_64);
  printf("Nombre de fois 128 : %d\n", cpt_128);
  printf("Nombre de fois 256 : %d\n", cpt_256);
  printf("Nombre de fois 512 : %d\n", cpt_512);
  printf("Nombre de fois 1024 : %d\n", cpt_1024);
  printf("Nombre de fois 2048 : %d\n", cpt_2048);
  printf("Nombre de fois 4096 : %d\n", cpt_4096);
}

/* static void display_grid(grid g){ */
/*   for (int i=0;i<GRID_SIDE;i++){ */
/*     for (int j=0;j<GRID_SIDE;j++){ */
/*       printf("%d ",get_tile(g,i,j)); */
/*     } */
/*     printf("\n"); */
/*   } */
/*   printf("\n"); */
/* } */
  

static long int maximum_tile(grid g){
  long int max_tile = 2;
  for(int i = 0; i<GRID_SIDE; i++){
    for(int j = 0; j< GRID_SIDE; j++){
      if(get_tile(g, i, j)>max_tile)
	max_tile = (long int)get_tile(g, i, j);
    }
  }
  return max_tile;
}




static tabBestM new_tabBestM(){
  tabBestM tab = malloc(sizeof(tabBestM*));
  assert(tab);
  tab->score = 0;
  tab->best = 0;
  return tab;
}



static dir best_move(grid g){
  grid test = new_grid();
  int empty = 0;
  tabBestM tab;
  tab  = new_tabBestM();
  for(dir i = UP; i<=RIGHT; i++){
    if(!can_move(g,i))
       continue;
    copy_grid(g,test);
    do_move(test,i);
    empty = empty_tiles(test);
    grid * tabG = malloc((empty*2)*sizeof(grid));
    assert(tabG);
    fill_tabG(empty,test, tabG);
    float sc = eval_tabG(empty,tabG);
    if(sc > tab->score){
      tab->score += sc;
      tab->best = i;
    }
    /* for(int i = 0;i<empty*2;i++) */
    /*   delete_grid(tabG[i]); */
    /* free(tabG); */
  }
  
  dir b = tab->best;
  //delete_grid(test);
  free(tab);
  return b;
}


static void fill_tabG(int empty, grid g, grid* tab){
  int i = 0;
  int n = empty*2-1;
  while(i < n){
    grid copy = new_grid();
    copy_grid(g,copy);
    for(int j = 0; j < 4; j++){
      for(int k = 0; k < 4; k++){
	if(get_tile(copy, j, k) == 0){
	  set_tile(copy, j, k, 1);
	  tab[i] = copy;
	  set_tile(copy, j, k, 2);
	  tab[n] = copy;
	  i++;
	  n--;
	}
      }
    }
  }
}



static float eval_branch(grid g){
  int over = 0;
  if(game_over(g))
    over = LOST_PENALTY;
  float mono = monotonicity(g);
  float smooth = smoothness(g)*SMOOTH_WEIGHT;
  float nb_merges = nb_merges_and_empty(g);
  return smooth + nb_merges - mono - over;
}


static int empty_tiles(grid g){
  int nb = 0;
  for(int i = 0; i<4;i++){
    for(int j = 0;j<4;j++){
      if(get_tile(g,i,j) == 0)
	nb += 1;
    }
  }
  return nb;
}


static float monotonicity(grid g){
  float mono_left = 0;
  float mono_right = 0;
  for(int i = 0;i<GRID_SIDE;i++)
    for(int j = 1;j<GRID_SIDE;j++)
      if(get_tile(g,i,j-1)>get_tile(g,i,j))
	mono_left = pow(get_tile(g,i,j-1),MONO_POW) - pow(get_tile(g,i,j),MONO_POW);
      else
	mono_right = pow(get_tile(g,i,j),MONO_POW) - pow(get_tile(g,i,j-1),MONO_POW); 
  if(mono_left > mono_right)
    return mono_right;
  return mono_left;
}


static long int smoothness(grid g){
  long int sum = 0;
  for(int i = 0;i<3;i++){
    for(int j = 0;j<3;j++){
      if(get_tile(g,i,j)>=get_tile(g,i,j))
	sum+=(long int)get_tile(g,i,j)-(long int)(get_tile(g,i,j+1));
      else
	sum+=(long int)get_tile(g,i,j+1)-(long int)(get_tile(g,i,j));
      if(get_tile(g,j,i)>=get_tile(g,j+1,i))
	sum+=(long int)get_tile(g,j,i)-(long int)(get_tile(g,j+1,i));
      else
	sum+=(long int)get_tile(g,j+1,i)-(long int)(get_tile(g,j,i));
    }
  }
  return sum;
}

static float nb_merges_and_empty(grid g){
  long int the_tile;
  long int prev = 0;
  int compt = 0;
  int empty = 0;
  float sum;
  int merges = 0;
  for(int i = 0;i<GRID_SIDE;i++){
    for(int j = 0;j<GRID_SIDE;j++){
      the_tile =(long int)get_tile(g,i,j);
      sum = pow(the_tile,SUM_POW);
      if(the_tile == 0)
	empty++;
      else{
	if(prev == the_tile)
	  compt++;
	else if(compt > 0){
	  merges += 1 + compt;
	  compt = 0;
	}
	prev = the_tile;
      }
    }
    if(compt > 0)
      merges += 1 + compt;
  }
  return (empty*EMPTY_WEIGHT)+(merges*MERGES_WEIGHT)-(sum*SUM_WEIGHT);
}


/* bool max_in_corner(grid g){ */
/*   long max_tile = maximum_tile(g); */
/*   return get_tile(g, 0, 0) == max_tile || get_tile(g, 3, 0) == max_tile || get_tile(g, 0, 3) == max_tile || get_tile(g, 3, 3) == max_tile; */
/* } */

static float eval_tabG(int empty,grid* tab){
  float score = 0;
  for(int i = 0; i < empty; i++)
    score += eval_branch(tab[i])*0.9;
  for(int i = empty; i < empty*2; i++)
    score += eval_branch(tab[i])*0.1;
  return score/(empty*10);
}  


/* static void display_grid(grid g); */
/* static void display_gameOver(bool *continuer, int *reponse_valide); */


/* int main(int argc, char *argv[]){ */
/*   keypad(stdscr, TRUE); */
/*   bool continuer = true; */
/*   srand(time(NULL)); */
/*   while(continuer){ */
/*     grid g = new_grid(); */
/*     add_tile(g); */
/*     add_tile(g); */
/*     int ch = 0; */
/*     display_grid(g); */
/*     int reponse_valide = 0; */
/*     while(!game_over(g) && reponse_valide == 0){ */
/*             ch=getch(); */
/*       switch(ch){ */
/*       case KEY_UP: */
	
/* 	  play(g, best_move(g)); */

/* 	break; */
/*       case 113: //"q" pour quitter */
/* 	continuer = false; */
/* 	reponse_valide = 1; */
/* 	break; */
/*       case 114: //"r" pour rejouer */
/* 	reponse_valide = 1; */
/* 	break; */
/*       } */
/*       display_grid(g); */
/*     } */
/*     while(reponse_valide == 0 && continuer == true) */
/*       display_gameOver(&continuer, &reponse_valide); */
/*     endwin(); */
/*   } */
/*   return EXIT_SUCCESS; */
/* } */


/* static void display_gameOver(bool *continuer, int *reponse_valide){ */
/*   keypad(stdscr, TRUE); */
/*   int ch = 0; */
/*   mvprintw(9, 45, "GAME OVER"); */
/*   mvprintw(11, 36, "Voulez-vous rejouer? y or n ? "); */
/*   refresh(); */
/*   cbreak(); */
/*   ch = getch(); */
/*   if(ch == 110){ */
/*     *continuer = false; // Arrete le programme et sort de la boucle */
/*     *reponse_valide = 1; */
/*   } */
/*   if(ch == 121) */
/*     *reponse_valide = 1; // Relance le programme du début */
/*   endwin(); */
/* } */

/* static void display_grid(grid g){ */
/*   initscr(); */
/*   clear(); */
/*   keypad(stdscr, TRUE); */
/*   // Mise en forme de la grille */
/*   int x = 3; */
/*   int y = 2; */
/*   for(int i = 0; i < 5; i++){ */
/*     mvhline(x, 3, ACS_HLINE, 31); */
/*     x += 4; */
/*   } */
/*   for(int i = 0; i < 5; i++){ */
/*     mvvline(4, y, ACS_VLINE, 15); */
/*     y += 8; */
/*   } */
/*   start_color(); */
/*   // Définition des couleurs (rouge, vert, jaune, bleu foncé, violet, bleu turquoise, blanc) */
/*   for(int i = 1; i < 8; i++){ */
/*     init_pair(i, i, 0); */
/*   } */
/*   // Mise des valeurs dans la grille */
/*   x = 5; */
/*   int t[] = {1,3,2,6,4,5,7}; */
/*   for(int i = 0; i < GRID_SIDE; i++){ */
/*     y = 5; */
/*     for(int j = 0; j < GRID_SIDE; j++){ */
/*       if(get_tile(g, j, i) != 0){ */
/* 	attron(COLOR_PAIR(t[(get_tile(g, j, i) - 1) % 7])); */
/* 	// Récupération d'un nombre et de la coloration */
/* 	char buff[5]; */
/* 	sprintf(buff, "%d", (unsigned int)pow(2, get_tile(g, j, i))); */
/* 	mvprintw(x, y, buff); */
/* 	attroff(COLOR_PAIR(1)); */
/*       } */
/*       y += 8; */
/*     } */
/*     x += 4; */
/*   } */
/*   mvprintw(1, 15, "2048"); */
/*   mvprintw(21, 3, "Recommencer Partie / Quitter Partie (r / q)"); */
/*   refresh(); */
/* } */

