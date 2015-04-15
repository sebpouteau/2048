#ifndef _FONCTION_TEST_H_
#define _FONCTION_TEST_H_

/**
 * \file fonction-test.h
 * \brief Contient les tests des fonctions du fichier grid.h
 **/

#include <stdbool.h>
#include <grid.h>

extern bool test_new_grid();
/**
 * \brief Verifie que la valeur retourne par get_tile est la bonne
 * \param g  Une grille pour faire le test
 * \return true si les valeurs correspondent false sinon
**/
extern bool test_get_tile(grid g);

/**
 * \brief Verifie si set_tile modifie correctement la valeur de la tile
 * \param g  Une grille pour faire le test
 * \return true si la valeur est correctement modifiee, false sinon
**/
extern bool test_set_tile(grid g);

/**
 * \brief Verifie que la grille copie correspond a la grille passee en parametre 
 * \param g  Une grille pour faire le test
 * \return true si la grille copiee correspond a la grille passee en parametre, false sinon
**/
extern bool test_copy_grid(grid g);

/**
 * \brief Verifie que get_score_grid renvoi le bon score correspondant a la grille passee en parametre 
 * \param g  Une grille pour faire le test
 * \return true si le score correspond, false sinon
**/
extern bool test_get_score_grid(grid g);

/**
 * \brief Verifie que que l'on obtient game_over si il n'y a ni case vide ni fusion possible 
 * \param g  Une grille pour faire le test
 * \return true si on obtient game_over, false sinon
**/
extern bool test_game_over(grid g);

/**
 * \brief Verifie que le mouvement vers le haut fonctionne correctement 
 * \param g  Une grille pour faire le test
 * \return true si apres le mouvement on obtient la grille souhaitait, false sinon
**/
extern bool test_do_move_up(grid g);

/**
 * \brief Verifie que le mouvement vers le bas fonctionne correctement 
 * \param g  Une grille pour faire le test
 * \return true si apres le mouvement on obtient la grille souhaitait, false sinon
**/
extern bool test_do_move_down(grid g);

/**
 * \brief Verifie que le mouvement vers la gauche fonctionne correctement 
 * \param g  Une grille pour faire le test
 * \return true si apres le mouvement on obtient la grille souhaitait, false sinon
**/
extern bool test_do_move_left(grid g);

/**
 * \brief Verifie que le mouvement vers la droite fonctionne correctement 
 * \param g  Une grille pour faire le test
 * \return true si apres le  mouvement on obtient la grille souhaitait, false sinon
**/
extern bool test_do_move_right(grid g);

/**
 * \brief Verifie que tous les mouvements les uns apres les autres fonctionnent correctement 
 * \param g  Une grille pour faire le test
 * \return true si apres les mouvement on obtient la grille souhaitait, false sinon
**/
extern bool test_do_move_all(grid g);

/**
 * \brief Verifie que can_move fonctionne 
 * \param g  Une grille pour faire le test
 * \return true si un mouvement est possible et can_move renvoi true et false si aucun mouvement possible et can_move renvoi false
**/
extern bool test_can_move(grid g);

/**
 * \brief Verifie que add_tile ajoute des tiles sur toutes la grille avec une probalite de 1/10 pour un 4 
 * \param g  Une grille pour faire le test
 * \return true si la grille est rempli en GRID_SIDE*GRID_SIDE ajout et la probalite de 4 est bonne, false sinon
**/
extern bool test_add_tile(grid g);

/**
 * \brief Verifie que le mouvement vers le haut fonctionne correctement 
 * \param g  Une grille pour faire le test
 * \return true si apres mouvement on obtient la grille souhaitait, false sinon
**/



#endif
