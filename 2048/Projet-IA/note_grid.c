#define MONO_WEIGHT 47
#define MONO_POW 4
#define SMOOTH_WEIGHT 40
#define EMPTY_WEIGHT 270
#define MERGES_WEIGHT 700
#define LOST_PENALTY 20000
#define SUM_WEIGHT 3.5
#define SUM_POW 11


static long int maximum_tile(grid g);
static float monotonicity(grid g);
static float nb_merges_and_empty(grid g);
static bool max_in_corner(grid g);
static float nb_merges_col(grid g);
static float note_grid(grid g);
//static long int smoothness(grid g);


static long int note_grid(grid g){
  long int max_corner = 0;
  int over = 0;
  long int max = maximum_tile(g);
  if(game_over(g))
    over = LOST_PENALTY;
  if(max_in_corner(g))
    max_corner = 1000*max;
  float mono = monotonicity(g);
  float nb_merges = nb_merges_and_empty(g);
  float nb_merges_c = nb_merges_col(g);
  return (long int)(nb_merges_c + nb_merges - mono - over + (float)max_corner);
} 


static float monotonicity(grid g){
  float mono_left = 0;
  float mono_right = 0;
  for(int i = 0;i<GRID_SIDE;i++)
    for(int j = 1;j<GRID_SIDE;j++){
      if(get_tile(g,i,j-1)>get_tile(g,i,j))
	mono_left = pow(get_tile(g,i,j-1),MONO_POW) - pow(get_tile(g,i,j),MONO_POW);
      else
	mono_right = pow(get_tile(g,i,j),MONO_POW) - pow(get_tile(g,i,j-1),MONO_POW); 
      if(get_tile(g,j-1,i)>get_tile(g,j,i))
	mono_left = pow(get_tile(g,j-1,i),MONO_POW) - pow(get_tile(g,j,i),MONO_POW);
      else
	mono_right = pow(get_tile(g,j,i),MONO_POW) - pow(get_tile(g,j-1,i),MONO_POW);
    }
  if(mono_left > mono_right)
    return mono_right;
  return mono_left;
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

static float nb_merges_col(grid g){
  long int the_tile;
  long int prev = 0;
  int compt = 0;
  int merges = 0;
  for(int i = 0;i<GRID_SIDE;i++){
    for(int j = 0;j<GRID_SIDE;j++){
      the_tile = (long int)get_tile(g,j,i);
      if(the_tile != 0){
	if(prev == the_tile)
	  compt++;
	else if(compt>0){
	  merges += 1 + compt;
	  compt = 0;
	}
	prev = the_tile;
      }
    }
    if(compt > 0)
      merges += 1 + compt;
  }
  return (float) merges*MERGES_WEIGHT;
}

static bool max_in_corner(grid g){
  long max_tile = maximum_tile(g);
  return get_tile(g, 0, 0) == max_tile || get_tile(g, 3, 0) == max_tile || get_tile(g, 0, 3) == max_tile || get_tile(g, 3, 3) == max_tile;
}

static long int maximum_tile(grid g){
  long int max_tile = 2;
  for(int i = 0; i<GRID_SIDE; i++){
    for(int j = 0; j< GRID_SIDE; j++){
      if(get_tile(g, i, j)>max_tile)
	max_tile =(long int) get_tile(g, i, j);
    }
  }
  return max_tile;
}


/* static long int smoothness(grid g){ */
/*   long int sum = 0; */
/*   for(int i = 0;i<3;i++){ */
/*     for(int j = 0;j<3;j++){ */
/*       if(get_tile(g,i,j)>=get_tile(g,i,j)) */
/* 	sum+=(long int)get_tile(g,i,j)-(long int)(get_tile(g,i,j+1)); */
/*       else */
/* 	sum+=(long int)get_tile(g,i,j+1)-(long int)(get_tile(g,i,j)); */
/*       if(get_tile(g,j,i)>=get_tile(g,j+1,i)) */
/* 	sum+=(long int)get_tile(g,j,i)-(long int)(get_tile(g,j+1,i)); */
/*       else */
/* 	sum+=(long int)get_tile(g,j+1,i)-(long int)(get_tile(g,j,i)); */
/*     } */
/*   } */
/*   return sum; */
/* } */
