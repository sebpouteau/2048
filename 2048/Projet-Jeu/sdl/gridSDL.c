#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "../src/grid.h"
#include "gridSDL.h"



// Affiche la grille
static void display_grid(grid g, SDL_Surface *surface_screen, SDL_Surface *surface_tile);

// Affiche le score
static void display_score(grid g, SDL_Surface *surface_screen, SDL_Surface *surface_score);

// Affiche le game over
static void display_gameover(grid g, SDL_Surface *surface_screen, SDL_Surface *surface_tile, SDL_Surface *surface_background_grid, bool *try_again);

// Affiche du texte celon les paramètres passés
static void display_text(char *char_text, int position_height, SDL_Surface *surface_screen, SDL_Surface *surface_text, TTF_Font *police_text, SDL_Color color_text, SDL_Color color_background, bool transparence);

// Récupère le pseudo saisi et l'écrit dans char_nickname
static void enter_nickname(char *char_nickname, char *char_highscore, SDL_Surface *surface_screen, SDL_Surface *surface_text, SDL_Surface *surface_tile, SDL_Surface *surface_background_grid, grid g, bool *end, bool *try_again);

// Lit une ligne dans un fichier
static void read_line(FILE *fichier, char *char_nickname, char *char_highscore);

// Ecrit une ligne dans un fichier
static void write_line(FILE *fichier, char *char_nickname, char *char_highscore);



// ====== FONCTIONS ========

void game_sdl(){
  // Initialisation de la fenetre du jeu
  SDL_Surface *surface_screen = NULL;
  surface_screen = SDL_SetVideoMode(500, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
  SDL_WM_SetCaption("Jeu 2048", NULL);
  SDL_FillRect(surface_screen, NULL, SDL_MapRGB(surface_screen->format, 255, 255, 255));

  // Initialisation du fond autour de la grille
  SDL_Surface *surface_background_grid = NULL;
  surface_background_grid = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_DOUBLEBUF, 420, 420, 32, 0, 0, 0, 0);
  SDL_Rect position_background_grid;
  position_background_grid.x = 40;
  position_background_grid.y = 40;
  SDL_FillRect(surface_background_grid, NULL, SDL_MapRGB(surface_screen->format, 0, 0, 255));
  SDL_BlitSurface(surface_background_grid, NULL, surface_screen, &position_background_grid);
  SDL_Flip(surface_screen);

  // Initialisation de la grille
  grid g = new_grid();
  add_tile(g);
  add_tile(g);
  
  // Paramètres boucle du jeu
  bool play_continue = true;
  bool try_again = false;
  SDL_Event event;

  // Paramètres affichage score
  SDL_Surface *surface_score = NULL;
  surface_score = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_DOUBLEBUF, 500, 600, 32, 0, 0, 0, 0);
  
  // Paramètres affichage grille
  SDL_Surface *surface_tile = NULL;

  // Affiche grille et score
  display_grid(g, surface_screen, surface_tile);
  display_score(g, surface_screen, surface_score);

  // Boucle du jeu
  while (play_continue){
    SDL_WaitEvent(&event);
    // Permet de quitter (en cliquant sur la croix pour fermer)
    if (event.type == SDL_QUIT)
      play_continue = false;
    // Choix de la direction à partir des touches directionnelles
    else if(event.type == SDL_KEYDOWN){
      switch(event.key.keysym.sym){
      case SDLK_UP:
	if(can_move(g, UP))
	  play(g,UP);
	break;
      case SDLK_DOWN:
	if(can_move(g, DOWN))
	  play(g,DOWN);
	break;
      case SDLK_LEFT:
	if(can_move(g, LEFT))
	  play(g,LEFT);
	break;
      case SDLK_RIGHT:
	if(can_move(g, RIGHT))
	  play(g,RIGHT);
	break;
      // Rejouer
      case SDLK_RETURN:
	try_again = true;
	play_continue = false;
	break;
      // Give Up
      case SDLK_ESCAPE:
	play_continue = false;
	break;
      default:
	break;
      }
    }
    // S'il y a des déplacements de souris, les ignorer
    else if(event.type == SDL_MOUSEMOTION)
      continue;

    display_grid(g, surface_screen, surface_tile);
    display_score(g, surface_screen, surface_score);

    if(game_over(g)){
      display_gameover(g, surface_screen, surface_tile, surface_background_grid, &try_again);
      play_continue = false;
    }
  }
  delete_grid(g);
  SDL_FreeSurface(surface_background_grid);
  SDL_FreeSurface(surface_score);
  SDL_FreeSurface(surface_screen);

  if(try_again)
    game_sdl();
}



static void display_grid(grid g, SDL_Surface *surface_screen, SDL_Surface *surface_tile){
  char name_tile[30];
  SDL_Rect position_tile;
  for(int i=0; i<GRID_SIDE; i++){
    for(int j=0; j<GRID_SIDE; j++){
      position_tile.x = 50 + i*100;
      position_tile.y = 50 + j*100;
      if(get_tile(g,i,j) == 0)
	sprintf(name_tile, "../sdl/tiles/tile0.bmp");
      else
	sprintf(name_tile, "../sdl/tiles/tile%d.bmp", (int)pow(2,get_tile(g,i,j)));
      surface_tile = SDL_LoadBMP(name_tile);
      SDL_BlitSurface(surface_tile, NULL, surface_screen, &position_tile);
    }
  }
  SDL_FreeSurface(surface_tile);
  SDL_FreeSurface(surface_screen);
  SDL_Flip(surface_screen);
}



static void display_score(grid g, SDL_Surface *surface_screen, SDL_Surface *surface_score){
  TTF_Font *police_text = TTF_OpenFont("../sdl/arial.ttf", 30);  
  SDL_Color color_text = {255, 0, 0}, color_background = {255,255,255};  
  char char_score[100];

  // Affiche le score
  sprintf(char_score, "Score : %lu ", grid_score(g));
  display_text(char_score, 5, surface_screen, surface_score, police_text, color_text, color_background, false);

  // Ouverture (et si besoin création) du fichier contenant l'highscore
  FILE* highscore_txt = fopen("../sdl/highscore_sdl.txt", "r+");
  if(highscore_txt == NULL){
    highscore_txt = fopen("../sdl/highscore_sdl.txt", "w");
  }

  // Récupère le pseudo et le highscore sauvegardé précédemment
  char char_highscore[10] = "";
  char char_nickname[10] = "";
  read_line(highscore_txt, char_nickname, char_highscore);
  // Convertis une chaine de caractère en unsigned long int
  unsigned long int highscore = strtoul(char_highscore, NULL, 10);

  // Affiche l'highscore
  if(grid_score(g) >= highscore){
    sprintf(char_highscore, "%lu", grid_score(g));
    write_line(highscore_txt, char_nickname, char_highscore);
    sprintf(char_score, "           New Highscore : %s !!           ", char_highscore);
    display_text(char_score, 470, surface_screen, surface_score, police_text, color_text, color_background, false);
  }else{
    sprintf(char_score, "Highscore :%s - %s", char_highscore, char_nickname);
    display_text(char_score, 470, surface_screen, surface_score, police_text, color_text, color_background, false);
  }

  // Affiche "Try Again"
  TTF_Font *police_menu = TTF_OpenFont("../sdl/arial.ttf", 25);
  char char_recommencer[30] = "Press ENTER to TRY AGAIN";
  display_text(char_recommencer, 520, surface_screen, surface_score, police_menu, color_text, color_background, false);

  // Affiche "Give Up"
  char char_giveup[30] = "or ESC to GIVE UP";
  display_text(char_giveup, 550, surface_screen, surface_score, police_menu, color_text, color_background, false);

  SDL_Flip(surface_screen);
  fclose(highscore_txt);
  SDL_FreeSurface(surface_screen);
  TTF_CloseFont(police_text);
  TTF_CloseFont(police_menu);
}



static void display_gameover(grid g, SDL_Surface *surface_screen, SDL_Surface *surface_tile, SDL_Surface *surface_background_grid, bool *try_again){ 
  SDL_Color color_text = {255, 255, 255}, color_background = {0, 0, 0};
  SDL_Rect position_background_grid;

  // Affiche "Game Over"
  SDL_Surface *surface_gameover = NULL;
  surface_gameover = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_DOUBLEBUF, 420, 420, 32, 0, 0, 0, 0);
  char *char_gameover = " GAME OVER ";
  TTF_Font *police_gameover = TTF_OpenFont("../sdl/arial.ttf", 55); 

  // Paramètre highscore
  bool end = true;
  char char_highscore[10];
  char char_nickname[10];
  FILE* highscore_txt = fopen("../sdl/highscore_sdl.txt", "r+"); 
  read_line(highscore_txt, char_nickname, char_highscore);
  // Convertis une chaine en unsigned long int
  unsigned long int highscore = strtoul(char_highscore, NULL, 10);


  // Réécriture de l'highscore si nouveau highscore
  if(grid_score(g) == highscore){
    char char_tmp_nickname[10]="";
    enter_nickname(char_tmp_nickname, char_highscore, surface_screen, surface_gameover, surface_tile, surface_background_grid, g, &end, try_again);
  }
  else{
    // Ajout d'un transparent sur la grille
    position_background_grid.x = 40;
    position_background_grid.y = 40;
    SDL_FillRect(surface_background_grid, NULL, SDL_MapRGB(surface_screen->format, 0, 0, 255));
    SDL_SetAlpha(surface_background_grid, SDL_SRCALPHA, 125);
    SDL_BlitSurface(surface_background_grid, NULL, surface_screen, &position_background_grid);
    // Affiche Game Over
    display_text(char_gameover, 220, surface_screen, surface_gameover, police_gameover, color_text, color_background, true);
  }

  SDL_Flip(surface_screen);
  // Boucle de fin
  SDL_Event event;
  while(end){
    SDL_WaitEvent(&event);
    if (event.type == SDL_QUIT){
      end = false;
    }
    else if(event.type == SDL_KEYDOWN){
      switch(event.key.keysym.sym){
      // Rejouer
      case SDLK_RETURN: 
	game_sdl();
	end = false;
	break;
      // Give Up
      case SDLK_ESCAPE:
	end = false;
	break;
      default:
	break;
      }
    }
  }

  fclose(highscore_txt);
  SDL_FreeSurface(surface_gameover);
  SDL_FreeSurface(surface_screen);
  TTF_CloseFont(police_gameover);
}



static void display_text(char *char_text, int position_height, SDL_Surface *surface_screen, SDL_Surface *surface_text, TTF_Font *police_text, SDL_Color color_text, SDL_Color color_background, bool transparence){
  SDL_Rect position_text;
  surface_text = TTF_RenderText_Shaded(police_text, char_text, color_text, color_background);
  if(transparence)
    SDL_SetColorKey(surface_text, SDL_SRCCOLORKEY, SDL_MapRGB(surface_text->format, 0, 0, 0));
  position_text.x = (surface_screen->w - surface_text->w)/2; // Centre le texte
  position_text.y = position_height;
  SDL_BlitSurface(surface_text, NULL, surface_screen, &position_text);
}



static void enter_nickname(char *char_nickname, char *char_highscore, SDL_Surface *surface_screen, SDL_Surface *surface_text, SDL_Surface *surface_tile, SDL_Surface *surface_background_grid, grid g, bool *end, bool *try_again){
  // Affiche la grille
  display_grid(g, surface_screen, surface_tile);

  // Ajout d'un transparent bleu sur la grille
  SDL_Rect position_background_grid;
  position_background_grid.x = 40;
  position_background_grid.y = 40;
  SDL_FillRect(surface_background_grid, NULL, SDL_MapRGB(surface_screen->format, 0, 0, 255));
  SDL_SetAlpha(surface_background_grid, SDL_SRCALPHA, 125);
  SDL_BlitSurface(surface_background_grid, NULL, surface_screen, &position_background_grid);
  SDL_Flip(surface_screen);
  
// Affiche Game Over
  TTF_Font *police_text = TTF_OpenFont("../sdl/arial.ttf", 30);
  TTF_Font *police_gameover = TTF_OpenFont("../sdl/arial.ttf", 55);
  SDL_Color color_text = {255, 255, 255}, color_background = {0, 0, 0};
  char *char_gameover = " GAME OVER ";
  char char_display[60] = "";
  char char_tmp[8] = "********";
  sprintf(char_display, "%s - ********", char_highscore);
  display_text(char_gameover, 115, surface_screen, surface_text, police_gameover, color_text, color_background, true);
  display_text(char_display, 355, surface_screen, surface_text, police_text, color_text, color_background, true);

  // Affiche complément du game over
  FILE* highscore_txt = fopen("../sdl/highscore_sdl.txt", "r+");
  char display_highscore[30] = "";
  sprintf(display_highscore, "New Highscore : %s !!", char_highscore);
  display_text(display_highscore, 210, surface_screen, surface_text, police_text, color_text, color_background, true);
  display_text("Enter your nickname :", 310, surface_screen, surface_text, police_text, color_text, color_background, true);
  
  // Met "Anyone" dans le pseudo par défaut
  write_line(highscore_txt, char_nickname, char_highscore);

  SDL_Flip(surface_screen);
  // Paramètre boucle while
  SDL_Event event;
  int cpt = 0;
  int num_char = 8;
  bool re_display = false;
  SDL_EnableUNICODE(1); // active l'unicode
  // Boucle while permettant de récupérer et de sauvegarder le pseudo saisi
  while(num_char >= cpt){
    SDL_WaitEvent(&event);
    if(event.type == SDL_QUIT){
      cpt = num_char+1;
      *end = false;
    }
    else if(event.type == SDL_KEYDOWN){
      switch(event.key.keysym.sym){
      // Give Up
      case SDLK_ESCAPE:
	cpt = num_char+1;
	*end = false;
	break;
      case SDLK_RETURN: // Touche "entrer"
	cpt = num_char+1;
	*end = false;
	*try_again = true;
	break;
      case SDLK_BACKSPACE: // Touche "supprimer"
	if(cpt>0){
	  char_nickname[strlen(char_nickname)-1] = '\0';
	  char_tmp[num_char - cpt] = '*';
	  if(cpt == 1)
	    sprintf(char_display, "%s - ********", char_highscore);
	  else
	    sprintf(char_display, "%s - %s%s", char_highscore, char_nickname, char_tmp);
	  write_line(highscore_txt, char_nickname, char_highscore);
	  re_display = true;
	  cpt -= 1;
	}
	break;
      default:
	// Si la touche est une lettre, un chiffre, ou un symbole
	if(event.key.keysym.unicode>=32 && event.key.keysym.unicode<=126){
	  if(num_char > cpt){
	    sprintf(char_nickname, "%s%c", char_nickname, (char)event.key.keysym.unicode);
	    char_tmp[num_char - cpt - 1] = '\0';
	    sprintf(char_display, "%s - %s%s", char_highscore, char_nickname, char_tmp);
	    write_line(highscore_txt, char_nickname, char_highscore);
	    re_display = true;
	    cpt += 1;
	  }
	}
	break;
      }
      if(re_display){
	// Affiche la grille
	display_grid(g, surface_screen, surface_tile);
	
	// Ajout d'un transparent sur la grille
	SDL_FillRect(surface_background_grid, NULL, SDL_MapRGB(surface_screen->format, 0, 0, 255));
	SDL_SetAlpha(surface_background_grid, SDL_SRCALPHA, 125);
	SDL_BlitSurface(surface_background_grid, NULL, surface_screen, &position_background_grid);
	
	// Affiche score et pseudo
	display_text(char_gameover, 115, surface_screen, surface_text, police_gameover, color_text, color_background, true);
	sprintf(display_highscore, "New Highscore : %s !!", char_highscore);
	display_text(display_highscore, 210, surface_screen, surface_text, police_text, color_text, color_background, true);
	display_text("Enter your nickname :", 310, surface_screen, surface_text, police_text, color_text, color_background, true);

	display_text(char_display, 355, surface_screen, surface_text, police_text, color_text, color_background, true);
	SDL_Flip(surface_screen);
	re_display = false;
      }
    }
    // S'il y a des déplacements de souris, les ignorer
    else if(event.type == SDL_MOUSEMOTION)
      continue;
  }
  SDL_EnableUNICODE(0); // désactive l'unicode

  TTF_CloseFont(police_text);
  TTF_CloseFont(police_gameover);
  fclose(highscore_txt);
}



static void read_line(FILE *fichier, char *char_nickname, char *char_highscore){
  rewind(fichier); 
  // Si le fichier n'est pas vide
  if(!feof(fichier)){
    fgets(char_nickname, 10, fichier);
    fgets(char_highscore, 10, fichier);
    for(int i=9; i>0; i--){
      if(char_nickname[i-1] == ' ')
	char_nickname[i-1] = '\0';
      else
	break;
    }
    for(int i=9; i>0; i--){
      if(char_highscore[i-1] == ' ')
	char_highscore[i-1] = '\0';
      else
	break;
    }
  }
}




static void write_line(FILE *fichier, char *char_nickname, char *char_highscore){
  rewind(fichier);
  // Si la chaine passé en paramètre est vide, on met "Anyone" par défaut.
  if(strlen(char_nickname) == 0){
    char_nickname = "Anyone";
  }
  fprintf(fichier, "%-10s%-10s", char_nickname, char_highscore);
}
