#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "../src/grid.h"
#include "../src/gridSDL.h"

// affiche la grille en SDL
static void display_grid_sdl(grid g, SDL_Surface *ecran, SDL_Surface *tile, SDL_Rect position_tile);

// affiche le score en SDL
static void display_score_sdl(grid g, SDL_Surface *ecran, SDL_Surface *texte_score, SDL_Rect position_score, SDL_Color color_score, SDL_Color color_background, TTF_Font *police_score, char *char_score, FILE *highscore_txt, unsigned long int highscore);

// affiche le game over en SDL
static void display_gameover_sdl(grid g, SDL_Surface *ecran);


void game_sdl(){
  // Initialisation de la fenetre du jeu
  SDL_Surface *ecran = NULL;
  ecran = SDL_SetVideoMode(400, 500, 32, SDL_HWSURFACE);
  SDL_WM_SetCaption("Jeu 2048", NULL);
  SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
  SDL_Flip(ecran);

  // Initialisation de la grille
  grid g = new_grid();
  add_tile(g);
  add_tile(g);
  
  //paramètres boucle du jeu
  bool continuer = true;
  SDL_Event event;

  //paramètres affichage score
  SDL_Surface *texte_score = NULL;
  SDL_Rect position_score;
  TTF_Font *police_score = TTF_OpenFont("angelina.TTF", 40);
  SDL_Color color_score = {255, 0, 0}, color_background ={255,255,255};
  char char_score[20] = "";

  //paramètre pour l'highscore
  FILE* highscore_txt = NULL; //fichier contenant l'highscore
  unsigned long int highscore = 0;

  //paramètres affichage grille
  SDL_Surface *tile = NULL;
  SDL_Rect position_tile;
  
  // boucle du jeu
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
    }
    if(game_over(g)){
      ecran = SDL_SetVideoMode(810, 455, 32, SDL_HWSURFACE);
      display_grid_sdl(g, ecran, tile, position_tile);
      display_gameover_sdl(g, ecran);
      continuer = 0;
    }
    display_grid_sdl(g, ecran, tile, position_tile);
    display_score_sdl(g, ecran, texte_score, position_score, color_score, color_background, police_score, char_score, highscore_txt, highscore);
  }
  SDL_FreeSurface(tile);
  SDL_FreeSurface(texte_score);
  SDL_FreeSurface(ecran);
  TTF_CloseFont(police_score);
}


static void display_grid_sdl(grid g, SDL_Surface *ecran, SDL_Surface *tile, SDL_Rect position_tile){
  for(int i=0; i<GRID_SIDE; i++){
    for(int j=0; j<GRID_SIDE; j++){
      position_tile.x = j*100;
      position_tile.y = i*100;
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
      SDL_BlitSurface(tile, NULL, ecran, &position_tile);
    }
  }
  SDL_Flip(ecran);
}


static void display_score_sdl(grid g, SDL_Surface *ecran, SDL_Surface *texte_score, SDL_Rect position_score, SDL_Color color_score, SDL_Color color_background, TTF_Font *police_score, char *char_score, FILE *highscore_txt, unsigned long int highscore){
  // on stock dans char_score "Score : " + grid_score(g)
  //** ("s" de sprintf pour "string")
  sprintf(char_score, "Score : %lu ", grid_score(g));
  texte_score = TTF_RenderText_Shaded(police_score, char_score, color_score, color_background);
  position_score.x = 50;
  position_score.y = 400;
  SDL_BlitSurface(texte_score, NULL, ecran, &position_score);
  SDL_Flip(ecran);  

  // "r+" = lecture et ecriture
  highscore_txt = fopen("highscore.txt", "r+");
  // on stocke dans "highscore", l'highscore mémorisé dans highscore_txt
  //** ("f" de fprintf pour file)
  fscanf(highscore_txt, "%lu", &highscore); 
  
  sprintf(char_score, "Highscore : %lu ", highscore); 
  texte_score = TTF_RenderText_Shaded(police_score, char_score, color_score, color_background);
  position_score.x = 50;
  position_score.y = 450;
  
  if(grid_score(g) > highscore){
    // on revient au début du fichier
    rewind(highscore_txt);
    // on écrit le nouveau highscore
    fprintf(highscore_txt, "%lu ", grid_score(g));
  }
  
  SDL_BlitSurface(texte_score, NULL, ecran, &position_score);
  SDL_Flip(ecran);
  fclose(highscore_txt);
}


static void display_gameover_sdl(grid g, SDL_Surface *ecran){
  bool end = true;
  SDL_Event event;
  SDL_Surface *game_over = NULL;
  SDL_Rect position_gameover;
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
