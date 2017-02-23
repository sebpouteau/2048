#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fonction-test.h>

/**
 * \file main-fonction-test.c
 * \brief main des fonctions de test
 **/


static int nbrTest = 1;
static void display_test(char *t, bool valide);


int main(int argc, char *argv[]){
  grid g = new_grid();
  display_test("new_grid", test_new_grid());
  display_test("get_tile", test_get_tile(g));
  display_test("set_tile", test_set_tile(g));
  display_test("get_score", test_get_score_grid(g));
  display_test("copy_grid", test_copy_grid(g));
  display_test("game_over", test_game_over(g));
  display_test("do_move_UP", test_do_move_up(g));
  display_test("do_move_DOWN", test_do_move_down(g));
  display_test("do_move_LEFT", test_do_move_left(g));
  display_test("do_move_RIGHT", test_do_move_right(g));
  display_test("do_move_ALL", test_do_move_all(g));
  display_test("test_can_move", test_can_move(g));
  display_test("test_add_tile", test_add_tile(g));
  delete_grid(g);
}


static void display_test(char *t, bool valide){
  printf("[TEST ");
  printf("%d]", nbrTest);
  int i = 0;
  if(nbrTest < 10)
    printf(" ");
  printf(" - ");
  while(t[i] != '\0'){
    printf("%c", t[i]);
    i++;
  }
  printf(" ");
  i++;
  while(i < 70){
    printf(".");
    i++;
  }
  nbrTest++;
  printf(valide ? " [\033[32;7m SUCCED \033[0m]" : " [\033[31;7m FAILED \033[0m]");
  printf("\n");
}
