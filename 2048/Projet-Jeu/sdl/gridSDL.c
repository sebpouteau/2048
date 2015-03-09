#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "../src/grid.h"
#include "gridSDL.h"


// affiche la grille
static void display_grid_sdl(grid g, SDL_Surface *ecran, SDL_Surface *surface_tile);

// affiche le score
static void display_score_sdl(grid g, SDL_Surface *ecran, SDL_Surface *surface_score);

// affiche le game over
static void display_gameover_sdl(grid g, SDL_Surface *ecran, SDL_Surface *surface_tile, SDL_Surface *fond_grid, bool try_again);

// affiche du texte
static void display_texte(char *char_texte, int position_x, int position_y, SDL_Surface *ecran, SDL_Surface *surface_texte, TTF_Font *police_texte, SDL_Color color_texte, SDL_Color color_background, bool transparence);

// récupère le pseudo saisi et l'écrit dans char_pseudo
static void saisir_pseudo(char *char_pseudo, int nbr_char, char *char_highscore, int position_x, int position_y, SDL_Surface *ecran, SDL_Surface *surface_texte, SDL_Surface *surface_tile, SDL_Surface *fond_grid, grid g, bool end, bool try_again);


// lit une ligne dans un fichier, de nb_char et la stocke dans char_line
static void read_line(FILE *fichier, char *char_pseudo, char *char_highscore);

// écrit une ligne dans un fichier.
static void write_line(FILE *fichier, char *char_pseudo, char *char_highscore);



// ====== FONCTIONS ========

void game_sdl(){
  // Initialisation de la fenetre du jeu
  SDL_Surface *ecran = NULL;
  ecran = SDL_SetVideoMode(500, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
  SDL_WM_SetCaption("Jeu 2048", NULL);
  SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));

  // Initialisation du fond autour de la grille
  SDL_Surface *fond_grid = NULL;
  SDL_Rect position_fond_grid;
  position_fond_grid.x = 40;
  position_fond_grid.y = 40;
  fond_grid = SDL_CreateRGBSurface(SDL_HWSURFACE, 420, 420, 32, 0, 0, 0, 0);
  SDL_FillRect(fond_grid, NULL, SDL_MapRGB(ecran->format, 0, 0, 255));
  SDL_BlitSurface(fond_grid, NULL, ecran, &position_fond_grid);
  SDL_Flip(ecran);

  // Initialisation de la grille
  grid g = new_grid();
  add_tile(g);
  add_tile(g);
  
  //paramètres boucle du jeu
  bool play_continue = true;
  bool try_again = false;
  SDL_Event event;

  //paramètres affichage score
  SDL_Surface *surface_score = NULL;
  
  //paramètres affichage grille
  SDL_Surface *surface_tile = NULL;

  display_grid_sdl(g, ecran, surface_tile);
  display_score_sdl(g, ecran, surface_score);

  // boucle du jeu
  while (play_continue){
    SDL_WaitEvent(&event);
    // permet de quitter
    if (event.type == SDL_QUIT)
      play_continue = false;
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
    // s'il y a des déplacements de souris, les ignorer
    else if(event.type == SDL_MOUSEMOTION)
      continue;

    display_grid_sdl(g, ecran, surface_tile);
    display_score_sdl(g, ecran, surface_score);

    if(game_over(g)){
      display_grid_sdl(g, ecran, surface_tile);
      display_score_sdl(g, ecran, surface_score);
      display_gameover_sdl(g, ecran, surface_tile, fond_grid, try_again);
      play_continue = false;
    }
  }
  SDL_FreeSurface(surface_tile);
  SDL_FreeSurface(surface_score);
  SDL_FreeSurface(ecran);

  if(try_again)
    game_sdl();
}




static void display_grid_sdl(grid g, SDL_Surface *ecran, SDL_Surface *surface_tile){
  char name_tile[30];
  SDL_Rect position_tile;
  for(int i=0; i<GRID_SIDE; i++){
    for(int j=0; j<GRID_SIDE; j++){
      position_tile.x = 50 + j*100;
      position_tile.y = 50 + i*100;
      sprintf(name_tile, "tiles/tile%d.bmp", get_tile(g,i,j));
      surface_tile = SDL_LoadBMP(name_tile);
      SDL_BlitSurface(surface_tile, NULL, ecran, &position_tile);
    }
  }
  SDL_Flip(ecran);
}




static void display_score_sdl(grid g, SDL_Surface *ecran, SDL_Surface *surface_score){
  TTF_Font *police_texte = TTF_OpenFont("arial.ttf", 30);  
  SDL_Color color_texte = {255, 0, 0}, color_background = {255,255,255};  
  char char_score[100];
  // Afficher le score
  sprintf(char_score, "Score : %lu ", grid_score(g));
  display_texte(char_score, (500/2)-(strlen(char_score)*6), 5, ecran, surface_score, police_texte, color_texte, color_background, false);

  // Afficher le highscore
  FILE* highscore_txt = fopen("highscore_sdl.txt", "r+"); //fichier contenant l'highscore

  char char_highscore[10] = "          ";
  char char_pseudo[10] = "          ";

  read_line(highscore_txt, char_pseudo, char_highscore);
  unsigned long int highscore = strtoul(char_highscore, NULL, 10); // convertir un chaine en unsigned long int

  if(grid_score(g) >= highscore){
    sprintf(char_highscore, "%lu", grid_score(g));
    write_line(highscore_txt, char_pseudo, char_highscore);
    sprintf(char_score, "     New Highscore : %s !!     ", char_highscore);
    display_texte(char_score, 260-(strlen(char_score)*6), 470, ecran, surface_score, police_texte, color_texte, color_background, false);
  }
  else{
    sprintf(char_score, "   Highscore :%s - %s   ", char_highscore, char_pseudo);
    display_texte(char_score, (500/2)-(strlen(char_score)*6.5), 470, ecran, surface_score, police_texte, color_texte, color_background, false);
  }

  // Afficher "recommencer"
  TTF_Font *police_menu = TTF_OpenFont("arial.ttf", 25);
  char char_recommencer[30] = "Press ENTER to RESTART";
  display_texte(char_recommencer, 105, 520, ecran, surface_score, police_menu, color_texte, color_background, false);

  // Afficher "Give Up - space"
  char char_giveup[30] = "or ESC to GIVE UP";
  display_texte(char_giveup, 150, 550, ecran, surface_score, police_menu, color_texte, color_background, false);

  SDL_Flip(ecran);
  fclose(highscore_txt);
  TTF_CloseFont(police_texte);
  TTF_CloseFont(police_menu);
}




static void display_gameover_sdl(grid g, SDL_Surface *ecran, SDL_Surface *surface_tile, SDL_Surface *fond_grid, bool try_again){
  TTF_Font *police_texte = TTF_OpenFont("arial.ttf", 30);  
  SDL_Color color_texte = {255, 255, 255}, color_background = {0, 0, 0};
  SDL_Rect position_fond_grid;
  // Ajout d'un transparent sur la grille
  SDL_FillRect(fond_grid, NULL, SDL_MapRGB(ecran->format, 0, 0, 255));
  SDL_SetAlpha(fond_grid, SDL_SRCALPHA, 125);
  SDL_BlitSurface(fond_grid, NULL, ecran, &position_fond_grid);
  SDL_Flip(ecran);

  SDL_Surface *surface_gameover = NULL;
  char *char_gameover = "  GAME OVER   ";
  display_texte(char_gameover, 140, 112, ecran, surface_gameover, police_texte, color_texte, color_background, true);

  bool end = true;

  char char_highscore[10];
  char char_pseudo[10];
  FILE* highscore_txt = fopen("highscore_sdl.txt", "r+"); 
  read_line(highscore_txt, char_pseudo, char_highscore);

  unsigned long int highscore = strtoul(char_highscore, NULL, 10); // convertir un chaine en unsigned long int

  if(grid_score(g) == highscore){
    highscore_txt = fopen("highscore_sdl.txt", "r+"); //fichier contenant l'highscore
    char char_tmp[10];
    saisir_pseudo(char_tmp, 8, char_highscore, 148, 355, ecran, surface_gameover, surface_tile, fond_grid, g, end, try_again);
    fclose(highscore_txt);
  }
  SDL_Flip(ecran);
  
  //boucle de fin
  SDL_Event event;
  while(end){
    SDL_WaitEvent(&event);
    if (event.type == SDL_QUIT)
      end = false;
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
  TTF_CloseFont(police_texte);
}



static void display_texte(char *char_texte, int position_x, int position_y, SDL_Surface *ecran, SDL_Surface *surface_texte, TTF_Font *police_texte, SDL_Color color_texte, SDL_Color color_background, bool transparence){
  SDL_Rect position_texte;
  surface_texte = TTF_RenderText_Shaded(police_texte, char_texte, color_texte, color_background);
  if(transparence)
    SDL_SetColorKey(surface_texte, SDL_SRCCOLORKEY, SDL_MapRGB(surface_texte->format, 0, 0, 0));
  position_texte.x = position_x;
  position_texte.y = position_y;
  SDL_BlitSurface(surface_texte, NULL, ecran, &position_texte);
  SDL_Flip(ecran);
}




static void saisir_pseudo(char *char_pseudo, int nbr_char, char *char_highscore, int position_x, int position_y, SDL_Surface *ecran, SDL_Surface *surface_texte, SDL_Surface *surface_tile, SDL_Surface *fond_grid, grid g, bool end, bool try_again){
  TTF_Font *police_texte = TTF_OpenFont("arial.ttf", 30);
  FILE* highscore_txt = fopen("highscore_sdl.txt", "r+"); //fichier contenant l'highscore
  SDL_Rect position_fond_grid;
  position_fond_grid.x = 40;
  position_fond_grid.y = 40;
  SDL_Color color_texte = {255, 255, 255}, color_background = {0, 0, 0};
  char display_highscore[30];

  display_grid_sdl(g, ecran, surface_tile);
  sprintf(display_highscore, "New Highscore : %s !!", char_highscore);
  display_texte(display_highscore, 80, 210, ecran, surface_texte, police_texte, color_texte, color_background, true);
  display_texte("Veuillez entrer votre pseudo :", 60, 310, ecran, surface_texte, police_texte, color_texte, color_background, true);

  SDL_Event event;
  int cpt = 0;
  char *char_gameover = "  GAME OVER   ";
  char char_display[60];
  char char_tmp[8] = "********";
  sprintf(char_display, "%s - ********", char_highscore);
  display_texte(char_display, position_x, position_y, ecran, surface_texte, police_texte, color_texte, color_background, true);

  SDL_EnableUNICODE(1); // active l'unicode
  while(nbr_char >= cpt){
    SDL_WaitEvent(&event);
    if(event.type == SDL_QUIT){
      cpt = nbr_char+1;
      end = false;
    }
    else if(event.type == SDL_KEYDOWN){
      switch(event.key.keysym.sym){
      case SDLK_RETURN:
	cpt = nbr_char+1;
	end = false;
	try_again = true;
	break;
      case SDLK_BACKSPACE:
	if(cpt>0){
	  char_pseudo[strlen(char_pseudo)-1] = '\0';
	  char_tmp[nbr_char - cpt] = '*';
	  if(cpt == 1)
	    sprintf(char_display, "%s - ********", char_highscore);
	  else
	    sprintf(char_display, "%s - %s%-10s", char_highscore, char_pseudo, char_tmp);
	  cpt -= 1;
	}
	break;
      default:

	if(event.key.keysym.unicode>=32 && event.key.keysym.unicode<=126){
	  if(nbr_char > cpt){
	    sprintf(char_pseudo, "%s%c", char_pseudo, (char)event.key.keysym.unicode);
	    char_tmp[nbr_char - cpt - 1] = '\0';
	    sprintf(char_display, "%s - %s%-10s", char_highscore, char_pseudo, char_tmp);

	    cpt += 1;
	  }
	}
	break;
      }
      // affiche grille
      display_grid_sdl(g, ecran, surface_tile);
      
      // Ajout d'un transparent sur la grille
      SDL_FillRect(fond_grid, NULL, SDL_MapRGB(ecran->format, 0, 0, 255));
      SDL_SetAlpha(fond_grid, SDL_SRCALPHA, 125);
      SDL_BlitSurface(fond_grid, NULL, ecran, &position_fond_grid);

      // Affiche score et pseudo
      display_texte(char_gameover, 140, 112, ecran, surface_texte, police_texte, color_texte, color_background, true);
      sprintf(display_highscore, "New Highscore : %s !!", char_highscore);
      display_texte(display_highscore, 80, 210, ecran, surface_texte, police_texte, color_texte, color_background, true);
      display_texte("Veuillez entrer votre pseudo :", 60, 310, ecran, surface_texte, police_texte, color_texte, color_background, true);
      display_texte(char_display, position_x, position_y, ecran, surface_texte, police_texte, color_texte, color_background, true);
      SDL_Flip(ecran);
      write_line(highscore_txt, char_pseudo, char_highscore);
    }
    // s'il y a des déplacements de souris, les ignorer
    else if(event.type == SDL_MOUSEMOTION)
      continue;
  }
  SDL_EnableUNICODE(0); // désactive l'unicode
  TTF_CloseFont(police_texte);
  fclose(highscore_txt);
}




static void read_line(FILE *fichier, char *char_pseudo, char *char_highscore){
  rewind(fichier); 
  if(!feof(fichier)){
    fgets(char_pseudo, 10, fichier);
    fgets(char_highscore, 10, fichier);
    for(int i=9; i>0; i--){
      if(char_pseudo[i-1] == ' ')
	char_pseudo[i-1] = '\0';
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




static void write_line(FILE *fichier, char *char_pseudo, char *char_highscore){
  rewind(fichier);
  if(strlen(char_pseudo) == 0){
    char_pseudo = "Anyone";
  }
  fprintf(fichier, "%-10s%-10s", char_pseudo, char_highscore);
}
