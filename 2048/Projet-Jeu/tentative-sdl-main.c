#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "grid.h"
 
static void jeu();
static void display_grid_sdl(grid g, SDL_Surface *ecran);

int main(int argc, char *argv[]){
  SDL_Surface *ecran = NULL;
  SDL_Init(SDL_INIT_VIDEO);
  ecran = SDL_SetVideoMode(400, 400, 32, SDL_HWSURFACE);
  grid g = new_grid();
  SDL_Flip(ecran);
  
  jeu(g, ecran);

  SDL_Quit();
  
  return EXIT_SUCCESS;
}

void jeu(grid g, SDL_Surface *ecran)
{
  bool continuer = true;
  SDL_Event event;
  
  while (continuer){
    SDL_WaitEvent(&event);
      switch(event.type){
      case SDL_QUIT:
	continuer = false;
      case SDL_KEYDOWN:
	switch(event.key.keysym.sym){
	case SDLK_UP:
	  play(g,UP);
	  break;
	case SDLK_DOWN:
	  play(g,DOWN);
	  break;
	case SDLK_LEFT:
	  play(g,LEFT);
	  break;
	case SDLK_RIGHT:
	  play(g,RIGHT);
	  break;
	default:
	  break;
	}
	break;
      default:
	break;
      }
    display_grid_sdl(g, ecran);
  }
}

static void display_grid_sdl(grid g, SDL_Surface *ecran){
  SDL_Surface *image = NULL;
  SDL_Rect position;
  for(int i=0; i<GRID_SIDE; i++){
    for(int j=0; j<GRID_SIDE; j++){
      position.x = j*100;
      position.y = i*100;
      switch(get_tile(g,i,j)){
      case 2:
	image = SDL_LoadBMP("tile2.bmp");
	break;
      case 4:
	image = SDL_LoadBMP("tile4.bmp");
	break;
      case 8:
	image = SDL_LoadBMP("tile8.bmp");
	break;
      case 16:
	image = SDL_LoadBMP("tile16.bmp");
	break;
      case 32:
	image = SDL_LoadBMP("tile32.bmp");
	break;
      case 0:
	image = SDL_LoadBMP("tile64.bmp");
	break;
	/*
      case 128:
	image = SDL_LoadBMP("tile128.bmp");
	break;
      case 256:
	image = SDL_LoadBMP("tile256.bmp");
	break;
      case 512:
	image = SDL_LoadBMP("tile512.bmp");
	break;
      case 1024:
	image = SDL_LoadBMP("tile1024.bmp");
	break;
      case 2048:
	image = SDL_LoadBMP("tile2048.bmp");
	break;
      case 4096:
	image = SDL_LoadBMP("tile4096.bmp");
	break;
      case 8192:
	image = SDL_LoadBMP("tile8192.bmp");
	break;
      case 16384:
	image = SDL_LoadBMP("tile16384.bmp");
	break;
      case 32768:
	image = SDL_LoadBMP("tile32768.bmp");
	break;

*/
      default:
	break;
      }
      SDL_BlitSurface(image, NULL, ecran, &position);
    }
  }
  SDL_Flip(ecran);
}


