#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "../src/grid.h"
#include "gridSDL.h"


// affiche la grille
static void display_grid_sdl(grid g, SDL_Surface *ecran, SDL_Surface *surface_tile, SDL_Rect position_tile, char *name_tile);

// affiche le score
static void display_score_sdl(grid g, SDL_Surface *ecran, SDL_Surface *surface_score, SDL_Rect position_score, SDL_Color color_score, SDL_Color color_background, TTF_Font *police_score, char *char_score, FILE *highscore_txt);

// affiche le game over
static void display_gameover_sdl(grid g, SDL_Surface *ecran, SDL_Color color_score, SDL_Color color_background, TTF_Font *police_score, FILE *highscore_txt);

// affiche du texte
static void display_texte(char *char_texte, int position_x, int position_y, SDL_Surface *ecran, SDL_Surface *surface_texte, SDL_Rect position_texte, TTF_Font *police_texte, SDL_Color color_texte, SDL_Color color_background);

// récupère le pseudo saisi et l'écrit dans char_pseudo
static void saisir_pseudo(char *char_pseudo, int nbr_char, char *char_highscore, int position_x, int position_y, SDL_Surface *ecran, SDL_Surface *surface_texte, SDL_Rect position_texte, TTF_Font *police_texte, SDL_Color color_texte, SDL_Color color_background);


// lit une ligne dans un fichier, de nb_char et la stocke dans char_line
static void read_line(FILE *fichier, char *char_pseudo, char *char_highscore);

// écrit une ligne dans un fichier.
static void write_line(FILE *fichier, char *char_pseudo, char *char_highscore);



// ====== FONCTIONS ========

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
  TTF_Font *police_score = TTF_OpenFont("arial.ttf", 30);
  SDL_Color color_score = {255, 0, 0}, color_background = {255,255,255};
  char char_score[100] = "";

  //paramètre pour l'highscore
  FILE* highscore_txt = fopen("highscore_sdl.txt", "r+"); //fichier contenant l'highscore
  
  //paramètres affichage grille
  SDL_Surface *surface_tile = NULL;
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
    display_grid_sdl(g, ecran, surface_tile, position_tile, name_tile);
    display_score_sdl(g, ecran, surface_score, position_score, color_score, color_background, police_score, char_score, highscore_txt);
    if(game_over(g)){
      ecran = SDL_SetVideoMode(850, 500, 32, SDL_HWSURFACE);
      SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
      display_grid_sdl(g, ecran, surface_tile, position_tile, name_tile);
      display_score_sdl(g, ecran, surface_score, position_score, color_score, color_background, police_score, char_score, highscore_txt);
      display_gameover_sdl(g, ecran, color_score, color_background, police_score, highscore_txt);
      continuer = 0;
    }
  }
  SDL_FreeSurface(surface_tile);
  SDL_FreeSurface(surface_score);
  SDL_FreeSurface(ecran);
  TTF_CloseFont(police_score);
}



static void display_grid_sdl(grid g, SDL_Surface *ecran, SDL_Surface *surface_tile, SDL_Rect position_tile, char *name_tile){
  for(int i=0; i<GRID_SIDE; i++){
    for(int j=0; j<GRID_SIDE; j++){
      position_tile.x = j*100;
      position_tile.y = i*100;
      sprintf(name_tile, "tiles/tile%d.bmp", get_tile(g,i,j));
      surface_tile = SDL_LoadBMP(name_tile);
      SDL_BlitSurface(surface_tile, NULL, ecran, &position_tile);
    }
  }
  SDL_Flip(ecran);
}

static void display_score_sdl(grid g, SDL_Surface *ecran, SDL_Surface *surface_score, SDL_Rect position_score, SDL_Color color_score, SDL_Color color_background, TTF_Font *police_score, char *char_score, FILE *highscore_txt){
  // Afficher le score
  sprintf(char_score, "Score : %lu ", grid_score(g));
  display_texte(char_score, 10, 400, ecran, surface_score, position_score, police_score, color_score, color_background);

  // Afficher le highscore
  highscore_txt = fopen("highscore_sdl.txt", "r+"); // "r+" = lecture et ecriture

  char char_highscore[10] = "          ";
  char char_pseudo[10] = "          ";

  read_line(highscore_txt, char_pseudo, char_highscore);
  unsigned long int highscore = strtoul(char_highscore, NULL, 10); // convertir un chaine en unsigned long int

  if(grid_score(g) >= highscore){
    sprintf(char_highscore, "%lu", grid_score(g));
    write_line(highscore_txt, char_pseudo, char_highscore);
    sprintf(char_score, "New Highscore : %s !!      ", char_highscore);
  }
  else
    sprintf(char_score, "Highscore : %s - %s ", char_highscore, char_pseudo);
  
  display_texte(char_score, 10, 450, ecran, surface_score, position_score, police_score, color_score, color_background);
  
  SDL_Flip(ecran);
  fclose(highscore_txt);
}

static void display_gameover_sdl(grid g, SDL_Surface *ecran, SDL_Color color_score, SDL_Color color_background, TTF_Font *police_score, FILE *highscore_txt){
  SDL_Surface *surface_gameover = NULL;
  SDL_Rect position;
  char *char_gameover = "\n\n\n\n\n\n\n\n\n\n\n";
  display_texte(char_gameover, 470, 70, ecran, surface_gameover, position, police_score, color_score, color_background);
  char_gameover = "\n  GAME OVER  \n";
  display_texte(char_gameover, 468, 100, ecran, surface_gameover, position, police_score, color_score, color_background);
  char_gameover = "\n\n\n\n\n\n\n\n\n\n\n";
  display_texte(char_gameover, 470, 130, ecran, surface_gameover, position, police_score, color_score, color_background);


  char char_highscore[10];
  char char_pseudo[10];

  highscore_txt = fopen("highscore_sdl.txt", "r+");
  read_line(highscore_txt, char_pseudo, char_highscore);

  unsigned long int highscore = strtoul(char_highscore, NULL, 10); // convertir un chaine en unsigned long int

  if(grid_score(g) == highscore){
    char display_highscore[30];
    fclose(highscore_txt);
    highscore_txt = fopen("highscore_sdl.txt", "w+");

    sprintf(display_highscore, "New Highscore : %s !!", char_highscore);
    display_texte(display_highscore, 450, 180, ecran, surface_gameover, position, police_score, color_score, color_background);
    display_texte("Veuillez entrer votre pseudo :", 420, 230, ecran, surface_gameover, position, police_score, color_score, color_background);
    char char_tmp[10];
    saisir_pseudo(char_tmp, 8, char_highscore, 500, 280, ecran, surface_gameover, position, police_score, color_score, color_background);
    write_line(highscore_txt, char_tmp, char_highscore);
    fclose(highscore_txt);
  }
  SDL_Flip(ecran);
  
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
}


static void display_texte(char *char_texte, int position_x, int position_y, SDL_Surface *ecran, SDL_Surface *surface_texte, SDL_Rect position_texte, TTF_Font *police_texte, SDL_Color color_texte, SDL_Color color_background){
  surface_texte = TTF_RenderText_Shaded(police_texte, char_texte, color_texte, color_background);
  position_texte.x = position_x;
  position_texte.y = position_y;
  SDL_BlitSurface(surface_texte, NULL, ecran, &position_texte);
  SDL_Flip(ecran);
}


static void saisir_pseudo(char *char_pseudo, int nbr_char, char *char_highscore, int position_x, int position_y, SDL_Surface *ecran, SDL_Surface *surface_texte, SDL_Rect position_texte, TTF_Font *police_texte, SDL_Color color_texte, SDL_Color color_background){
  SDL_Event event;
  int cpt = 0;

  char char_display[60];
  char char_tmp[8] = "********";
  sprintf(char_display, "%s - ********", char_highscore);
  display_texte(char_display, position_x, position_y, ecran, surface_texte, position_texte, police_texte, color_texte, color_background);

  SDL_EnableUNICODE(1); // active l'unicode
  while(nbr_char > cpt){
    SDL_WaitEvent(&event);
    if(event.type == SDL_QUIT)
      cpt = nbr_char;
    else if(event.type == SDL_KEYDOWN){
      switch(event.key.keysym.sym){
      case SDLK_RETURN:
	cpt = nbr_char;
	break;
      default:
	if(event.key.keysym.unicode>=32 && event.key.keysym.unicode<=126){
	  sprintf(char_pseudo, "%s%c", char_pseudo, (char)event.key.keysym.unicode);
	  char_tmp[nbr_char - cpt - 1] = '\0';
	  sprintf(char_display, "%s - %s%-10s", char_highscore, char_pseudo, char_tmp);
	  display_texte(char_display, position_x, position_y, ecran, surface_texte, position_texte, police_texte, color_texte, color_background);
	  cpt += 1;
	}
	break;
      }
    }
  }
  SDL_EnableUNICODE(0); // désactive l'unicode
}


static void read_line(FILE *fichier, char *char_pseudo, char *char_highscore){
  rewind(fichier); 
  if(!feof(fichier)){
    fscanf(fichier, "%10s", char_pseudo);
    fscanf(fichier, "%10s", char_highscore);
    for(int i=10; i>0; i--){
      if(char_pseudo[i-1] == ' ')
	char_pseudo[i-1] = '\0';
    }
    for(int i=10; i>0; i--){
      if(char_highscore[i-1] == ' ')
	char_highscore[i-1] = '\0';
    }
  }
}

static void write_line(FILE *fichier, char *char_pseudo, char *char_highscore){
  rewind(fichier);
  if(strlen(char_pseudo) == 0){
    char_pseudo = "aaa";
  }
  fprintf(fichier, "%-10s%-10s", char_pseudo, char_highscore);
}
