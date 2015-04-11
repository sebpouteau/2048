#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <grid.h>
#include <strategy.h>
#include <math.h>

#define NOMBRE_TEST 1

strategy A2_Emery_Gouraud_Kirov_Pouteau_low();
strategy A2_Emery_Gouraud_Kirov_Pouteau_low();

static long maximum_tile(grid g){
  long max_tile = 1;
  for(int i = 0; i<GRID_SIDE; i++){
    for(int j = 0; j< GRID_SIDE; j++){
      if(get_tile(g, i, j)>max_tile)
	max_tile = get_tile(g, i, j);
    }
  }
  return max_tile;
}

static void usage(char * commande){
  fprintf(stderr,"%s <nombre repetition > \n", commande);
  exit(EXIT_FAILURE);
}

int main (int argc, char **argv){
  if (argc != 2)     
    usage(argv[0]);
  int n = atoi(argv[1]);
  int nb_lance = n;
  int cpt_16 = 0;
  int cpt_32 = 0;
  int cpt_64 = 0;
  int cpt_128 = 0;
  int cpt_256 = 0;
  int cpt_512 = 0;
  int cpt_1024 = 0;
  int cpt_2048 = 0;
  int cpt_4096 = 0;
  int cpt_8192= 0;
  
  srand(time(NULL));
  strategy str = A2_Emery_Gouraud_Kirov_Pouteau_low();
  while(n > 0){
    grid g = new_grid();
    add_tile(g);
    add_tile(g);
    
    while(!game_over(g)){
      dir d = str->play_move(str,g);
      if (can_move(g,d))
	play(g, d);
    }
    printf("max %lu\n",(long int)pow(2,maximum_tile(g)));
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
    if(maximum_tile(g) == 13)
      cpt_8192 += 1;
    delete_grid(g);
    n -= 1;
  }
  str->free_strategy(str);
  printf("\n --------------- \n");
  printf("Sur %d lancés : \n\n", nb_lance);
  printf("Nombre de fois 16 : %d\n", cpt_16);
  printf("Nombre de fois 32 : %d\n", cpt_32);
  printf("Nombre de fois 64 : %d\n", cpt_64);
  printf("Nombre de fois 128 : %d\n", cpt_128);
  printf("Nombre de fois 256 : %d\n", cpt_256);
  printf("Nombre de fois 512 : %d\n", cpt_512);
  printf("Nombre de fois 1024 : %d\n", cpt_1024);
  printf("Nombre de fois 2048 : %d\n", cpt_2048);
  printf("Nombre de fois 4096 : %d\n", cpt_4096);
  printf("Nombre de fois 8192 : %d\n", cpt_8192);
}


