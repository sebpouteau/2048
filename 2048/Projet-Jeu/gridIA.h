#ifndef _GRIDIA_H_
#define _GRIDIA_H_

#include <stdbool.h>

/* retourne le tableau g->case_empty, qui contient toutes les cases vides */
tile* get_grid_case_empty(grid g);

/* retourne le nombre de case vide */
int get_nbr_case_empty(grid g);

/* retourne si l'on peut déplacer une ligne ou une colonne */
bool possible_lc(grid g, int i, int j, int a,int b);

/* retourne le nombre de fusion possible dans la direction indiqué */
int poids_fusion(grid g, dir d)

/* retourne la différence entre le score de la grille avant le déplacement,
   et aprés le déplacement */
int poids_fusion(grid g, dir d)

#endif