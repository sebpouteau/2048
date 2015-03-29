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
static void display_grid(grid g, SDL_Surface *surface_screen);

// Affiche le score
static void display_score(grid g, SDL_Surface *surface_screen);

// Affiche le game over
static void display_gameover(grid g, SDL_Surface *surface_screen, bool *try_again);

// Affiche du texte selon les paramètres passés
static void display_text(SDL_Surface *surface_screen, char *char_text, int position_height, TTF_Font *police_text, SDL_Color color_text, SDL_Color color_background, bool transparence);

// Récupère le pseudo saisi et l'écrit dans char_nickname
static void enter_nickname(grid g, SDL_Surface *surface_screen, char *char_highscore, bool *end_game, bool *try_again);

// Lit la première ligne dans un fichier
static void read_line(FILE *fichier, char *char_nickname, char *char_highscore);

// Ecrit la première ligne dans un fichier
static void write_line(FILE *fichier, char *char_nickname, char *char_highscore);



// ====== FONCTIONS ========

void game_sdl(){
  // Initialisation de la fenetre du jeu
  SDL_Surface *surface_screen = NULL;
  surface_screen = SDL_SetVideoMode(500, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
  SDL_WM_SetCaption("Game 2048 - by Emery, Gouraud, Kirov & Pouteau", NULL);
  SDL_FillRect(surface_screen, NULL, SDL_MapRGB(surface_screen->format, 255, 255, 255));

  // Initialisation du contour bleu foncé autour de la grille
  SDL_Surface *surface_background_grid = NULL; 
  surface_background_grid = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_DOUBLEBUF, 420, 420, 32, 0, 0, 0, 0);
  SDL_Rect position_background_grid;
  position_background_grid.x = 40;
  position_background_grid.y = 40;
  SDL_FillRect(surface_background_grid, NULL, SDL_MapRGB(surface_screen->format, 0, 0, 255));
  SDL_BlitSurface(surface_background_grid, NULL, surface_screen, &position_background_grid);

  // Initialisation de la grille
  grid g = new_grid();
  add_tile(g);
  add_tile(g);
  
  // Paramètres boucle du jeu
  bool play_continue = true; // Booléan de la boucle du jeu
  bool try_again = false; // Booléan permettant de relancer un nouveau jeu
  SDL_Event event;

  // Affiche grille et score
  display_grid(g, surface_screen);
  display_score(g, surface_screen);
  SDL_Flip(surface_screen);

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
	  play(g, UP);
	break;
      case SDLK_DOWN:
	if(can_move(g, DOWN))
	  play(g, DOWN);
	break;
      case SDLK_LEFT:
	if(can_move(g, LEFT))
	  play(g, LEFT);
	break;
      case SDLK_RIGHT:
	if(can_move(g, RIGHT))
	  play(g, RIGHT);
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

    SDL_FillRect(surface_screen, NULL, SDL_MapRGB(surface_screen->format, 255, 255, 255));
    SDL_BlitSurface(surface_background_grid, NULL, surface_screen, &position_background_grid);
    display_grid(g, surface_screen);
    display_score(g, surface_screen);
    SDL_Flip(surface_screen);

    if(game_over(g)){
      display_gameover(g, surface_screen, &try_again);
      play_continue = false;
    }
  }

  // Libère la mémoire allouée
  delete_grid(g);
  SDL_FreeSurface(surface_background_grid);
  SDL_FreeSurface(surface_screen);

  // Relance un nouveau jeu si le joueur l'a décidé
  if(try_again)
    game_sdl();
}



static void display_grid(grid g, SDL_Surface *surface_screen){
  SDL_Surface *surface_tile = NULL;
  SDL_Rect position_tile;
  char name_tile[30];
  for(int i = 0; i < GRID_SIDE; i++){
    for(int j = 0; j < GRID_SIDE; j++){
      position_tile.x = 50 + i*100;
      position_tile.y = 50 + j*100;
      if(get_tile(g, i, j) == 0)
	sprintf(name_tile, "../sdl/tiles/tile0.bmp");
      else
	sprintf(name_tile, "../sdl/tiles/tile%d.bmp", (int)pow(2, get_tile(g, i, j)));
      surface_tile = SDL_LoadBMP(name_tile);
      SDL_BlitSurface(surface_tile, NULL, surface_screen, &position_tile);
      SDL_FreeSurface(surface_tile);
    }
  }
}



static void display_score(grid g, SDL_Surface *surface_screen){
  // Paramètres affichage score
  TTF_Font *police_text = TTF_OpenFont("../sdl/arial.ttf", 30);  
  SDL_Color color_text = {255, 0, 0}; // Couleur rouge
  SDL_Color color_background = {255, 255, 255}; // Couleur blanche
  char char_score[100];

  // Ouverture (et si besoin création) du fichier contenant l'highscore
  FILE* highscore_txt = fopen("../sdl/highscore_sdl.txt", "r+");
  if(highscore_txt == NULL)
    highscore_txt = fopen("../sdl/highscore_sdl.txt", "w");

  // Récupère le pseudo et le highscore sauvegardés précédemment
  char char_highscore[10] = "";
  char char_nickname[10] = "";
  read_line(highscore_txt, char_nickname, char_highscore);
  unsigned long int highscore = strtoul(char_highscore, NULL, 10); // Convertit une chaine de caractère en unsigned long int

  // Affiche le score
  sprintf(char_score, "Score : %lu", grid_score(g));
  display_text(surface_screen,char_score, 5, police_text, color_text, color_background, false);

  // Affiche l'highscore
  if(grid_score(g) >= highscore){
    sprintf(char_highscore, "%lu", grid_score(g));
    write_line(highscore_txt, char_nickname, char_highscore);
    sprintf(char_score, "New Highscore : %s !!", char_highscore);
    display_text(surface_screen, char_score, 470, police_text, color_text, color_background, false);
  }else{
    sprintf(char_score, "Highscore :%s - %s", char_highscore, char_nickname);
    display_text(surface_screen, char_score, 470, police_text, color_text, color_background, false);
  }

  // Affiche "Try Again"
  TTF_Font *police_menu = TTF_OpenFont("../sdl/arial.ttf", 25);
  char char_recommencer[30] = "Press ENTER to TRY AGAIN";
  display_text(surface_screen, char_recommencer, 520, police_menu, color_text, color_background, false);

  // Affiche "Give Up"
  char char_giveup[30] = "or ESC to GIVE UP";
  display_text(surface_screen, char_giveup, 550, police_menu, color_text, color_background, false);

  // Libère la mémoire allouée et ferme le fichier de Highscore
  TTF_CloseFont(police_text);
  TTF_CloseFont(police_menu);
  fclose(highscore_txt);
}



static void display_gameover(grid g, SDL_Surface *surface_screen, bool *try_again){
  // Paramètres couleur texte
  SDL_Color color_text = {255, 255, 255}; // Couleur blanche
  SDL_Color color_background = {0, 0, 0}; // Couleur noire

  // Paramètres Game Over
  char *char_gameover = "GAME OVER";
  TTF_Font *police_gameover = TTF_OpenFont("../sdl/arial.ttf", 55); 

  // Paramètres Highscore
  bool end_game = true; // Booléan de la boucle de fin du jeu
  char char_highscore[10]; // Chaine de caractère contenant le highscore
  char char_nickname[10]; // Chaine de caractère contenant le pseudo
  FILE* highscore_txt = fopen("../sdl/highscore_sdl.txt", "r+"); 
  read_line(highscore_txt, char_nickname, char_highscore);
  fclose(highscore_txt);
  unsigned long int highscore = strtoul(char_highscore, NULL, 10); // Convertit une chaine en unsigned long int

  // Affiche la grille
  display_grid(g, surface_screen);

  // Ajout d'un transparent bleu sur la grille
  SDL_Surface *surface_background_grid = NULL;
  surface_background_grid = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_DOUBLEBUF, 420, 420, 32, 0, 0, 0, 0);
  SDL_Rect position_background_grid;
  position_background_grid.x = 40;
  position_background_grid.y = 40;
  SDL_FillRect(surface_background_grid, NULL, SDL_MapRGB(surface_screen->format, 0, 0, 255));
  SDL_SetAlpha(surface_background_grid, SDL_SRCALPHA, 125);
  SDL_BlitSurface(surface_background_grid, NULL, surface_screen, &position_background_grid);
  SDL_Flip(surface_screen);

  // Réécriture de l'Highscore si new highscore
  if(grid_score(g) == highscore)
    enter_nickname(g, surface_screen, char_highscore, &end_game, try_again);
  else
    display_text(surface_screen, char_gameover, 220, police_gameover, color_text, color_background, true); // Affiche "Game Over"

  SDL_Flip(surface_screen);
  
  // Boucle de fin
  SDL_Event event;
  while(end_game){
    SDL_WaitEvent(&event);
    if (event.type == SDL_QUIT){
      end_game = false;
    }
    else if(event.type == SDL_KEYDOWN){
      switch(event.key.keysym.sym){
      // Rejouer
      case SDLK_RETURN:
	*try_again = true;
	end_game = false;
	break;
      // Give Up
      case SDLK_ESCAPE:
	end_game = false;
	break;
      default:
	break;
      }
    }
  }

  // Libère la mémoire allouée
  TTF_CloseFont(police_gameover);
  SDL_FreeSurface(surface_background_grid);
}



static void display_text(SDL_Surface *surface_screen, char *char_text, int position_height, TTF_Font *police_text, SDL_Color color_text, SDL_Color color_background, bool transparence){
  // Paramètres affichage du texte
  SDL_Surface *surface_text = NULL;
  SDL_Rect position_text;
  surface_text = TTF_RenderText_Shaded(police_text, char_text, color_text, color_background);

  // Met en transparent la couleur du background du texte
  if(transparence)
    SDL_SetColorKey(surface_text, SDL_SRCCOLORKEY, SDL_MapRGB(surface_text->format, 0, 0, 0));

  // Affiche le texte
  position_text.x = (surface_screen->w - surface_text->w)/2; // Centre le texte
  position_text.y = position_height;
  SDL_BlitSurface(surface_text, NULL, surface_screen, &position_text);
  
  // Libère la mémoire allouée
  SDL_FreeSurface(surface_text);
}



static void enter_nickname(grid g, SDL_Surface *surface_screen, char *char_highscore, bool *end_game, bool *try_again){
  // Paramètres transparent bleu
  SDL_Surface *surface_background_grid = NULL;
  surface_background_grid = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_DOUBLEBUF, 420, 420, 32, 0, 0, 0, 0);
  SDL_Rect position_background_grid;
  position_background_grid.x = 40;
  position_background_grid.y = 40;
  SDL_FillRect(surface_background_grid, NULL, SDL_MapRGB(surface_screen->format, 0, 0, 255));
  SDL_SetAlpha(surface_background_grid, SDL_SRCALPHA, 125);

  // Paramètres d'affichage du Game Over et du texte
  FILE* highscore_txt = fopen("../sdl/highscore_sdl.txt", "r+"); // Ouvre le fichier highscore_sdl.txt
  TTF_Font *police_text = TTF_OpenFont("../sdl/arial.ttf", 30);
  TTF_Font *police_gameover = TTF_OpenFont("../sdl/arial.ttf", 55);
  SDL_Color color_text = {255, 255, 255}; // Couleur blanche
  SDL_Color color_background = {0, 0, 0}; // Couleur noire
  char char_nickname[10]= ""; // Chaine de caractère qui contiendra le nouveau pseudo
  char *char_gameover = "GAME OVER";
  char char_display[60] = ""; // Chaine de caractère qui contiendra "char_highscore - char_nickname" 
  char char_tmp[8] = "********"; // Chaine de caractère permettant au joueur de visualiser le nombre de caractère restant pour écrire le pseudo
  char char_newHighscore[30] = ""; // Chaine de caractère qui contiendra "New Highscore - char_highscore"

  // Affiche Game Over
  sprintf(char_display, "%s - %s", char_highscore, char_tmp);
  display_text(surface_screen, char_gameover, 115, police_gameover, color_text, color_background, true);
  display_text(surface_screen, char_display, 355, police_text, color_text, color_background, true);

  // Affiche complément du game over
  sprintf(char_newHighscore, "New Highscore : %s !!", char_highscore);
  display_text(surface_screen, char_newHighscore, 210, police_text, color_text, color_background, true);
  display_text(surface_screen, "Enter your nickname :", 310, police_text, color_text, color_background, true);

  // Permet de mettre comme pseudo par défaut "Anyone"
  write_line(highscore_txt, char_nickname, char_highscore);

  SDL_Flip(surface_screen);

  // Paramètre boucle while
  SDL_Event event;
  int cpt_char = 0; // Nombre de caractère saisi par l'utilisateur
  int num_max_char = 8; // Nombre maximum de caractère pour le pseudo
  bool re_display = false; // Booléan de réaffichage de l'écran
  bool enter_new_nickname = true; // Booléan de la boucle de saisi du nouveau pseudo
  SDL_EnableUNICODE(1); // active l'unicode

  // Boucle while permettant de récupérer et de sauvegarder le pseudo saisi dans le fichier hghscore_sdl.txt
  while(enter_new_nickname){
    SDL_WaitEvent(&event);
    if(event.type == SDL_QUIT){
      enter_new_nickname = false;
      *end_game = false;
    }
    else if(event.type == SDL_KEYDOWN){
      switch(event.key.keysym.sym){
      // Give Up
      case SDLK_ESCAPE:
	enter_new_nickname = false;
	*end_game = false;
	break;
      case SDLK_RETURN: // Touche "entrée"
	enter_new_nickname = false;
	*end_game = false;
	*try_again = true;
	break;
      case SDLK_BACKSPACE: // Touche "supprimer"
	if(cpt_char > 0){
	  char_nickname[strlen(char_nickname) - 1] = '\0';
	  char_tmp[num_max_char - cpt_char] = '*';
	  re_display = true;
	  cpt_char -= 1;
	}
	break;
      default:
	if(num_max_char > cpt_char){
	  // Si la touche est une lettre, un chiffre, ou un symbole
	  if(event.key.keysym.unicode >= 32 && event.key.keysym.unicode <= 126){
	    sprintf(char_nickname, "%s%c", char_nickname, (char)event.key.keysym.unicode);
	    char_tmp[num_max_char - cpt_char - 1] = '\0';
	    re_display = true;
	    cpt_char += 1;
	  }
	}
	break;
      }

      // Réaffiche tous les éléments liés au Game Over, ainsi que le nouveau pseudo saisi
      if(re_display){
	// Modifie l'affichage du nouveau pseudo et écrit le nouveau pseudo dans highscore_txt
	sprintf(char_display, "%s - %s%s", char_highscore, char_nickname, char_tmp);
	write_line(highscore_txt, char_nickname, char_highscore);

	// Affiche la grille
	display_grid(g, surface_screen);
	
	// Ajout d'un transparent bleu sur la grille
	SDL_BlitSurface(surface_background_grid, NULL, surface_screen, &position_background_grid);
	
	// Affiche score
	display_text(surface_screen, char_gameover, 115, police_gameover, color_text, color_background, true);
	sprintf(char_newHighscore, "New Highscore : %s !!", char_highscore);
	display_text(surface_screen, char_newHighscore, 210, police_text, color_text, color_background, true);
	display_text(surface_screen, "Enter your nickname :", 310, police_text, color_text, color_background, true);

	// Affiche pseudo saisi
	display_text(surface_screen, char_display, 355, police_text, color_text, color_background, true);

	SDL_Flip(surface_screen);
	re_display = false;
      }
    }
    // S'il y a des déplacements de souris, les ignorer
    else if(event.type == SDL_MOUSEMOTION)
      continue;
  }

  SDL_EnableUNICODE(0); // désactive l'unicode

  // Libère la mémoire allouée et ferme le fichier stockant le Highscore
  SDL_FreeSurface(surface_background_grid);
  TTF_CloseFont(police_text);
  TTF_CloseFont(police_gameover);
  fclose(highscore_txt);
}



static void read_line(FILE *fichier, char *char_nickname, char *char_highscore){
  // Revient au début du fichier
  rewind(fichier);

  // Si le fichier n'est pas vide
  if(!feof(fichier)){
    fgets(char_nickname, 10, fichier);
    fgets(char_highscore, 10, fichier);
    for(int i = 9; i > 0; i--){
      if(char_nickname[i-1] == ' ')
	char_nickname[i-1] = '\0';
      else
	break;
    }
    for(int i = 9; i > 0; i--){
      if(char_highscore[i-1] == ' ')
	char_highscore[i-1] = '\0';
      else
	break;
    }
  }
}




static void write_line(FILE *fichier, char *char_nickname, char *char_highscore){
  // Revient au début du fichier
  rewind(fichier);

  // Si la chaine passée en paramètre est vide, on écrit "Anyone" par défaut dans le pseudo
  if(strlen(char_nickname) == 0){
    char_nickname = "Anyone";
  }

  fprintf(fichier, "%-10s%-10s", char_nickname, char_highscore);
}
