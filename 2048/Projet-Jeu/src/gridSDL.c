#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "grid.h"
#include "gridComplementaire.h"
#include "gridSDL.h"


// affiche la grille
static void display_grid_sdl(grid g, SDL_Surface *ecran, SDL_Surface *tile, SDL_Rect position_tile, char *name_tile);

// affiche le score
static void display_score_sdl(grid g, SDL_Surface *ecran, SDL_Surface *surface_score, SDL_Rect position_score, SDL_Color color_score, SDL_Color color_background, TTF_Font *police_score, char *char_score, FILE *highscore_txt, unsigned long int highscore);

// affiche le game over
static void display_gameover_sdl(grid g, SDL_Surface *ecran, SDL_Color color_score, SDL_Color color_background, TTF_Font *police_score, FILE *highscore_txt, unsigned long int highscore);

// affiche du texte
static void display_texte(char *char_texte, int position_x, int position_y, SDL_Surface *ecran, SDL_Surface *surface_texte, SDL_Rect position_texte, TTF_Font *police_texte, SDL_Color color_texte, SDL_Color color_background);

  
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
  SDL_Surface *surface_score = NULL;
  SDL_Rect position_score;
  TTF_Font *police_score = TTF_OpenFont("angelina.TTF", 40);
  SDL_Color color_score = {255, 0, 0}, color_background = {255,255,255};
  char char_score[20] = "";

  //paramètre pour l'highscore
  FILE* highscore_txt = fopen("highscore_sdl.txt", "r+"); //fichier contenant l'highscore
  unsigned long int highscore = 0;
  fscanf(highscore_txt, "%lu", &highscore);

  //paramètres affichage grille
  SDL_Surface *tile = NULL;
  char name_tile[30];
  SDL_Rect position_tile;
  
  // boucle du jeu
  while (continuer){
    SDL_WaitEvent(&event);
    // permet de quitter
    if (event.type == SDL_QUIT)
      continuer = false;
    // choix direction avec touche directionnelle
    else if(event.type == SDL_KEYDOWN){
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
    }
    display_grid_sdl(g, ecran, tile, position_tile, name_tile);
    display_score_sdl(g, ecran, surface_score, position_score, color_score, color_background, police_score, char_score, highscore_txt, highscore);
    if(game_over(g)){
      ecran = SDL_SetVideoMode(800, 500, 32, SDL_HWSURFACE);
      SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
      display_grid_sdl(g, ecran, tile, position_tile, name_tile);
      display_score_sdl(g, ecran, surface_score, position_score, color_score, color_background, police_score, char_score, highscore_txt, highscore);
      display_gameover_sdl(g, ecran, color_score, color_background, police_score, highscore_txt, highscore);
      continuer = 0;
    }
  }
  SDL_FreeSurface(tile);
  SDL_FreeSurface(surface_score);
  SDL_FreeSurface(ecran);
  TTF_CloseFont(police_score);
}



static void display_grid_sdl(grid g, SDL_Surface *ecran, SDL_Surface *tile, SDL_Rect position_tile, char *name_tile){
  for(int i=0; i<GRID_SIDE; i++){
    for(int j=0; j<GRID_SIDE; j++){
      position_tile.x = j*100;
      position_tile.y = i*100;
      sprintf(name_tile, "../../tiles/tile%d.bmp", get_tile(g,i,j));
      tile = SDL_LoadBMP(name_tile);
      SDL_BlitSurface(tile, NULL, ecran, &position_tile);
    }
  }
  SDL_Flip(ecran);
}

static void display_score_sdl(grid g, SDL_Surface *ecran, SDL_Surface *surface_score, SDL_Rect position_score, SDL_Color color_score, SDL_Color color_background, TTF_Font *police_score, char *char_score, FILE *highscore_txt, unsigned long int highscore){
  // Afficher le score
  sprintf(char_score, "Score : %lu ", grid_score(g));
  display_texte(char_score, 50, 400, ecran, surface_score, position_score, police_score, color_score, color_background);

  // Afficher le score
  highscore_txt = fopen("highscore_sdl.txt", "r+"); // "r+" = lecture et ecriture
  fscanf(highscore_txt, "%lu", &highscore); 
  if(grid_score(g) > highscore){
    rewind(highscore_txt); // on revient au début du fichier
    fprintf(highscore_txt, "%lu ", grid_score(g));
  }
  sprintf(char_score, "Highscore : %lu ", highscore); 
  display_texte(char_score, 50, 450, ecran, surface_score, position_score, police_score, color_score, color_background);
  
  SDL_Flip(ecran);
  fclose(highscore_txt);
}

static void display_gameover_sdl(grid g, SDL_Surface *ecran, SDL_Color color_score, SDL_Color color_background, TTF_Font *police_score, FILE *highscore_txt, unsigned long int highscore){
  SDL_Surface *surface_gameover = NULL;
  SDL_Rect position;
  char *char_gameover = " GAME OVER ";
  display_texte(char_gameover, 500, 100, ecran, surface_gameover, position, police_score, color_score, color_background);
  
  highscore_txt = fopen("highscore_sdl.txt", "r+");
  fscanf(highscore_txt, "%lu", &highscore);
  
  if(highscore == grid_score(g)){
    char char_highscore[30] = "";
    sprintf(char_highscore, "New Highscore : %lu", highscore);
    display_texte(char_highscore, 450, 150, ecran, surface_gameover, position, police_score, color_score, color_background);
    char char_pseudo[30] = "Veuillez entrer votre pseudo :";
    display_texte(char_pseudo, 420, 200, ecran, surface_gameover, position, police_score, color_score, color_background);
  }
  
  bool end = true;
  SDL_Event event;
  while(end){
    SDL_WaitEvent(&event);
    switch(event.type){
    case SDL_QUIT:
      end = false;
      break;
    }
  }
  fclose(highscore_txt);
}


static void display_texte(char *char_texte, int position_x, int position_y, SDL_Surface *ecran, SDL_Surface *surface_texte, SDL_Rect position_texte, TTF_Font *police_texte, SDL_Color color_texte, SDL_Color color_background){
  surface_texte = TTF_RenderText_Shaded(police_texte, char_texte, color_texte, color_background);
  position_texte.x = position_x;
  position_texte.y = position_y;
  SDL_BlitSurface(surface_texte, NULL, ecran, &position_texte);
  SDL_Flip(ecran);
}
