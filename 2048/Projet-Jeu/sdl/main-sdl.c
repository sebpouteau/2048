#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <time.h>
#include "gridSDL.h"

int main(int argc, char *argv[]){
  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();
  
  game_sdl();

  TTF_Quit();
  SDL_Quit();
  
  return EXIT_SUCCESS;
}
