#ifndef _GRIDCOMPLEMENTAIRE_H_
#define _GRIDCOMPLEMENATIREp_H_

#include <stdbool.h>


void move( grid g,int i, int j,int a, int b );
void fusion (grid g,int i,int j,int a,int b);
void grid_case_empty(grid g);
bool possible(grid g, int i,int j,int a,int b);


#endif
