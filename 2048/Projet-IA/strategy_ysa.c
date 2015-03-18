#include <stdio.h>
#include <stdlib.h>
#include "../Projet-Jeu/src/grid.h"
#include "strategy.h"
#include <math.h>
#include <time.h>

dir stratsimple(strategy s, grid g);
strategy init_strategy();
int max_grid(grid g);
void free_memless_strat (strategy strat)
{
  free (strat);
}

int main(int argc, char* argv[]){
  strategy s = init_strategy();
  srand(time(NULL));
   for(int i = 0; i < 20; i++){
     grid g = new_grid();
     add_tile(g);
     add_tile(g);
     while(!game_over(g)){
       dir d = stratsimple(s, g);
       play(g, d);
     }
     printf("max: %d\n", max_grid(g));

   }
}

dir stratsimple(strategy s, grid g){
    if(can_move(g, UP))
      return UP;
    if(can_move(g, LEFT))
      return LEFT;
    if(!can_move(g, LEFT) && !can_move(g, UP) && can_move(g, RIGHT))
      return RIGHT;
    return DOWN;
}

strategy init_strategy(){
  strategy s = malloc(sizeof(strategy));
  s->mem = NULL;
  s->name = "stratsimple";
  s->play_move = stratsimple;
  s->free_strategy = free_memless_strat;
  return s;
}

 int max_grid(grid g){
   int max = get_tile(g, 0, 0);
   for(int i = 0; i < GRID_SIDE; i++)
     for(int j = 0; i < GRID_SIDE; j++)
       if(get_tile(g, j, i) > max)
	 max = get_tile(g, j, i);
   return pow(2, max);
 }
#include "strategy.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
void free_memless_strat (strategy strat)
{
free (strat);
}

/* if (can_move(g,UP)){return UP;} */
/* else if (can_move(g,LEFT)){return LEFT;} */
/* else if (can_move(g,DOWN)){return DOWN;} */
/* else if (can_move(g,RIGHT)){return RIGHT;} */

dir strategy_seb(strategy str, grid g){
dir direction;


return direction;
}

long int note_grid (g){
long int cpt = 0;
int cpt_case_empty = 0;
for(int y = 0; y < GRID_SIDE; y++){
for(int x = 0; x < GRID_SIDE; x++){
cpt += get_tile(g,x,y);
if(get_tile(g,x,y) == 0)
cpt_case_empty++;
}
}
int max = maximum_tile(g);
if( get_tile(g, 0, 0) == max ||
}


strategy init_Structure (){
strategy str = malloc (sizeof(strategy));
str->name = "strategy_seb";
str->mem = NULL;
str->free_strategy = free_memless_strat;
str->play_move = strategy_seb;
return str;
}

static bool objectif_atteint(grid g);
static long maximum_tile(grid g);

int main(int argc,char **argv){


int n = 10;
srand(time(NULL));

while(n >0){
grid g= new_grid();
add_tile(g);
add_tile(g);
//int ch=0;
strategy seb = init_Structure();
dir direction;
while(!game_over(g)){
direction = strategy_seb(seb,g);
play(g,direction);
}
printf("Objectif atteint? %s \n", objectif_atteint(g)?"oui":"non");
printf("Tile max = %ld \n",(long int)pow(2, maximum_tile(g)));
delete_grid(g);
n -=1;
}
}
