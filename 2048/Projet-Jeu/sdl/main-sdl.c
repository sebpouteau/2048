#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "../src/grid.h"
#include "../src/gridSDL.h"


int main(int argc, char *argv[]){
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();
  
  game_sdl();

  TTF_Quit();
  SDL_Quit();
  
  return EXIT_SUCCESS;
}
