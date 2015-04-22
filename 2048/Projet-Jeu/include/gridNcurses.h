#ifndef _GRIDNCURSES_H_
#define _GRIDNCURSES_H_
/**
 * \brief affiche la grille
 * \param g grille
 * \return 
 */
extern void display_grid(grid g);

/**
 * \brief affiche le game_over
 * \param *continue_game,*valid_answer pointeur sur un bool
 * \return 
 */
extern void display_game_over(bool *continue_game, int *valid_answer);

/**
 * \brief recupere le highscore
 * \return unsigned long int
 */
extern unsigned long int read_highscore();

/**
 * \brief ecrit le highscore
 * \param score le score a ecrire
 * \return
 */
extern void write_highscore(unsigned long int score);
#endif
