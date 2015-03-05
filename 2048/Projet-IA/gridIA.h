#ifndef _GRIDIA_H_
#define _GRIDIA_H_

#include <stdbool.h>

/* retourne la différence entre le score de la grille avant le déplacement,
   et aprés le déplacement */
unsigned long int poids_fusion(grid g, dir d);

/* retourne la différence entre le score de la grille avant le déplacement,
   et aprés le déplacement et ajoute la valeur de la tile en haut à gauche */
unsigned long int poids_fusion_jimmy(grid g, dir d);

/* retourne le nombre de fusion possible dans la direction indiqué */
int nombre_fusion(grid g,dir d);

/* retourne si l'on peut déplacer une ligne ou une colonne */
bool possible_lc(grid g, int i, int j, int a,int b);

/* retourne true si deux grilles sont identiques*/
bool egalite_grid(grid g,grid g1);

#endif
