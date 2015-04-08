#ifndef _FONCTION_TEST_H_
#define _FONCTION_TEST_H_

/**
 * \file fonction-test.h
 * \brief Contient les tests des fonctions du fichier grid.h
 **/

#include <stdbool.h>
#include <grid.h>

extern bool test_get_tile(grid g);
extern bool test_set_tile(grid g);
extern bool test_copy_grid(grid g);
extern bool test_get_score_grid(grid g);
extern bool test_game_over(grid g);
extern bool test_do_move_up(grid g);
extern bool test_do_move_down(grid g);
extern bool test_do_move_left(grid g);
extern bool test_do_move_right(grid g);
extern bool test_do_move_all(grid g);
extern bool test_can_move(grid g);
extern bool test_add_tile(grid g);
extern bool test_delete_grid();
extern bool test_new_grid();

#endif