#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "../src/grid.h"
 
static void jeu();
static void display_grid_sdl(grid g, SDL_Surface *ecran);
static void display_gameover(grid g, SDL_Surface *ecran);

int main(int argc, char *argv[]){
  SDL_Surface *ecran = NULL;
  SDL_Init(SDL_INIT_VIDEO);
  ecran = SDL_SetVideoMode(400, 500, 32, SDL_HWSURFACE);
  grid g = new_grid();
  add_tile(g);
  add_tile(g);
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
	break;
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
      }/*
    if(game_over(g)){
      display_gameover(g, ecran);
      continuer = 0;
      }*/
    display_grid_sdl(g, ecran);
  }
}

  
static void display_gameover(grid g, SDL_Surface *ecran){
  bool end = true;
  SDL_Event event;
  SDL_Surface *game_over = NULL;
  SDL_Rect position_gameover;
  ecran = SDL_SetVideoMode(810, 455, 32, SDL_HWSURFACE);
  display_grid_sdl(g, ecran);
  position_gameover.x = 0;
  position_gameover.y = 0;
  game_over = SDL_LoadBMP("../../tiles/gameover.bmp");
  SDL_SetColorKey(game_over, SDL_SRCCOLORKEY, SDL_MapRGB(game_over->format, 255, 255, 255));
  SDL_BlitSurface(game_over, NULL, ecran, &position_gameover);
  SDL_Flip(ecran);
  while(end){
    SDL_WaitEvent(&event);
    switch(event.type){
    case SDL_QUIT:
      end = false;
      break;
    }
  }
}

static void display_grid_sdl(grid g, SDL_Surface *ecran){
  SDL_Surface *tile = NULL;
  SDL_Rect position;
  for(int i=0; i<GRID_SIDE; i++){
    for(int j=0; j<GRID_SIDE; j++){
      position.x = j*100;
      position.y = i*100;
      switch(get_tile(g,i,j)){
      case 0:
	tile = SDL_LoadBMP("../../tiles/tile0.bmp");
	break;
      case 2:
	tile = SDL_LoadBMP("../../tiles/tile2.bmp");
	break;
      case 4:
	tile = SDL_LoadBMP("../../tiles/tile4.bmp");
	break;
      case 8:
	tile = SDL_LoadBMP("../../tiles/tile8.bmp");
	break;
      case 16:
	tile = SDL_LoadBMP("../../tiles/tile16.bmp");
	break;
      case 32:
	tile = SDL_LoadBMP("../../tiles/tile32.bmp");
	break;
      case 64:
	tile = SDL_LoadBMP("../../tiles/tile64.bmp");
	break;
      case 128:
	tile = SDL_LoadBMP("../../tiles/tile128.bmp");
	break;
      case 256:
	tile = SDL_LoadBMP("../../tiles/tile256.bmp");
	break;
      case 512:
	tile = SDL_LoadBMP("../../tiles/tile512.bmp");
	break;
      case 1024:
	tile = SDL_LoadBMP("../../tiles/tile1024.bmp");
	break;
      case 2048:
	tile = SDL_LoadBMP("../../tiles/tile2048.bmp");
	break;
      case 4096:
	tile = SDL_LoadBMP("../../tiles/tile4096.bmp");
	break;
      case 8192:
	tile = SDL_LoadBMP("../../tiles/tile8192.bmp");
	break;
      case 16384:
	tile = SDL_LoadBMP("../../tiles/tile16384.bmp");
	break;
      case 32768:
	tile = SDL_LoadBMP("../../tiles/tile32768.bmp");
	break;
      default:
	break;
      }
      SDL_BlitSurface(tile, NULL, ecran, &position);
    }
  }
  SDL_Flip(ecran);
}


