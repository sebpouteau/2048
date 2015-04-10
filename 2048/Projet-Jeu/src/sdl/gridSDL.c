#define _XOPEN_SOURCE // permet d'utiliser la fonction putenv() qui centre la fenetre
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_getenv.h>
#include <grid.h>
#include <gridSDL.h>


#define PATH_TILE "../src/sdl/tiles/"
#define PATH_ANIMATION "../src/sdl/animation_penguin/"
#define PATH_BUTTON_MENU "../src/sdl/menu_button/"
#define PATH_HIGHSCORE "../src/sdl/highscore_sdl.txt"
#define PATH_POLICE "../src/sdl/arial.ttf"

#define TILE_SIDE 100 // Taille de la tuile
#define WINDOW_MENU_WIDTH 4 * TILE_SIDE // Largeur de la fenetre
#define WINDOW_MENU_HEIGHT 5 * TILE_SIDE // Hauteur de la fenetre
#define WINDOW_WIDTH ((GRID_SIDE == 2 ? 3 : GRID_SIDE)  + 1) * TILE_SIDE // Largeur de la fenetre
#define WINDOW_HEIGHT (GRID_SIDE + 2) * TILE_SIDE // Hauteur de la fenetre
#define POSITION_TILE_X (WINDOW_WIDTH - GRID_SIDE * TILE_SIDE)/2 // Position abscisse de la grille
#define POSITION_TILE_Y ((WINDOW_HEIGHT - (1 + GRID_SIDE) * TILE_SIDE)/2 + 10) // Position ordonnee de la grille 
#define POSITION_BACKGROUND_X (POSITION_TILE_X - 10) // Position abscisse du background de la grille
#define POSITION_BACKGROUND_Y (POSITION_TILE_Y - 10) // Position ordonnee du background de la grille


// variable global pour la couleur des tuiles
static char *char_color;


// Affiche le menu
static void display_menu(SDL_Surface *surface_screen, SDL_Surface *surface_background_grid, int animation);

// Affiche la grille
static void display_grid(grid g, SDL_Surface *surface_screen);

// Affiche le score
static void display_score(grid g, SDL_Surface *surface_screen);

// Affiche le game over
static void display_gameover(grid g, SDL_Surface *surface_screen, SDL_Surface *surface_background_grid, SDL_Rect position_background_grid, bool *try_again);

// Affiche du texte selon les parametres passes
static void display_text(SDL_Surface *surface_screen, TTF_Font *police_text, SDL_Color color_text, SDL_Color color_background, char *char_text, int position_height, bool transparence);

// Recupere le pseudo saisi et l'ecrit dans char_nickname
static void enter_nickname(grid g, SDL_Surface *surface_screen, SDL_Surface *surface_background_grid, SDL_Rect position_background_grid, char *char_highscore, bool *end_game, bool *try_again);

// Lit la premiere ligne dans un fichier
static void read_line(FILE *fichier, char *char_nickname, char *char_highscore);

// Ecrit la premiere ligne dans un fichier
static void write_line(FILE *fichier, char *char_nickname, char *char_highscore);


// ====== FONCTIONS ========

void game_sdl(){
  // Initialisation de la fenetre du jeu
  SDL_Surface *surface_screen = NULL;
  putenv("SDL_VIDEO_WINDOW_POS=center"); // Permet de centrer la fenetre du jeu
  SDL_WM_SetCaption("Game 2048 - by Emery, Gouraud, Kirov & Pouteau", NULL);

  // Initialisation du contour autour de la grille (sa couleur dépendra du choix de l'utilisateur)
  SDL_Surface *surface_background_grid = NULL; 
  surface_background_grid = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_DOUBLEBUF, 20 + TILE_SIDE * GRID_SIDE, 20 + TILE_SIDE * GRID_SIDE, 32, 0, 0, 0, 0);
  SDL_Rect position_background_grid;
  position_background_grid.x = POSITION_BACKGROUND_X;
  position_background_grid.y = POSITION_BACKGROUND_Y;

  // Initialisation de la grille
  grid g = new_grid();
  add_tile(g);
  add_tile(g);

  // Parametres boucle du jeu
  bool game_loop = true; // Boolean de la boucle du jeu
  bool try_again = false; // Boolean permettant de relancer un nouveau jeu
  bool menu = true; // Boolean pour le menu
  bool game = false; // Boolean pour le jeu
  bool change_color = false;
  int current_time = 0, before_time = 0; // Permet de gerer le temps pour l'animation
  int num_animation = 0; // numéro de l'animation
  SDL_Event event;
 
  // Boucle du jeu
  while(game_loop){
    if(menu){
      current_time = SDL_GetTicks();
      if(current_time - before_time > 100){
	before_time = current_time;
	num_animation++;
	if(num_animation > 16)
	  num_animation = 1;
	display_menu(surface_screen, surface_background_grid, num_animation);
      }
    }

    if(SDL_PollEvent(&event)){
      // Permet de quitter (en cliquant sur la croix pour fermer)
      if(event.type == SDL_QUIT)
	game_loop = false;
      else if(event.type == SDL_KEYDOWN){
	switch(event.key.keysym.sym){
	  // Choix de la direction a partir des touches directionnelles
	case SDLK_UP:
	  if(game && can_move(g, UP))
	    play(g, UP);
	  break;
	case SDLK_DOWN:
	  if(game && can_move(g, DOWN))
	    play(g, DOWN);
	  break;
	case SDLK_LEFT:
	  if(game && can_move(g, LEFT))
	    play(g, LEFT);
	  break;
	case SDLK_RIGHT:
	  if(game && can_move(g, RIGHT))
	    play(g, RIGHT);
	  break;
	  // Rejouer
	case SDLK_RETURN:
	  try_again = true;
	  game_loop = false;
	  break;
	  // Quitter la partie
	case SDLK_ESCAPE:
	  game_loop = false;
	  break;
	  // Choix de la couleur des tuiles
	case SDLK_F1:
	  char_color = "green/";
	  change_color = true;
	  break;
	case SDLK_F2:
	  char_color = "red/";
	  change_color = true;
	  break;
	case SDLK_F3:
	  char_color = "blue/";
	  change_color = true;
	  break;
	default:
	  break;
	}
      }
    }
    // S'il y a des deplacements de souris, les ignorer
    else if(event.type == SDL_MOUSEMOTION)
      continue;
    
    if(change_color){
      if(menu)
	surface_screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
      // strcmp compare deux chaines de caracteres et retourne 0 si elle sont identiques
      if(strcmp(char_color, "green/") == 0)
	SDL_FillRect(surface_background_grid, NULL, SDL_MapRGB(surface_screen->format, 0, 150, 50)); // vert foncé
      else if(strcmp(char_color, "red/") == 0)
	SDL_FillRect(surface_background_grid, NULL, SDL_MapRGB(surface_screen->format, 255, 0, 0));
      else if(strcmp(char_color, "blue/") == 0)  
	SDL_FillRect(surface_background_grid, NULL, SDL_MapRGB(surface_screen->format, 0, 0, 255));
      menu = false;
      game = true;
      change_color = false;
    }

    if(game){
      // Affiche le jeu
      SDL_FillRect(surface_screen, NULL, SDL_MapRGB(surface_screen->format, 255, 255, 255));
      SDL_BlitSurface(surface_background_grid, NULL, surface_screen, &position_background_grid);
      display_grid(g, surface_screen);
      display_score(g, surface_screen);
      SDL_Flip(surface_screen);
    }

    if(game_over(g)){
      display_gameover(g, surface_screen, surface_background_grid, position_background_grid, &try_again);
      game_loop = false;
    }
  }

  // Libere la memoire allouee
  delete_grid(g);
  SDL_FreeSurface(surface_background_grid);
  SDL_FreeSurface(surface_screen);

  // Relance un nouveau jeu si le joueur l'a decide
  if(try_again)
    game_sdl();
}



static void display_menu(SDL_Surface *surface_screen, SDL_Surface *surface_background_grid, int num_animation){
  surface_screen = SDL_SetVideoMode(400, 500, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
  SDL_FillRect(surface_screen, NULL, SDL_MapRGB(surface_screen->format, 200, 200, 200));

  // Paramètres des boutons
  char char_button[50];
  SDL_Surface *surface_button;
  SDL_Rect position_button;
  int nb_button = 4;

  for(int i = 1; i < nb_button; i++){
    if(i == 1)
      sprintf(char_button, "%sF1_Green.bmp", PATH_BUTTON_MENU);
    if(i == 2)
      sprintf(char_button, "%sF2_Red.bmp", PATH_BUTTON_MENU);
    if(i == 3)
      sprintf(char_button, "%sF3_Blue.bmp", PATH_BUTTON_MENU);
    surface_button = SDL_LoadBMP(char_button);
    position_button.x = (WINDOW_MENU_WIDTH - surface_button->w) / 2;
    position_button.y = i * WINDOW_MENU_HEIGHT / nb_button;
    SDL_BlitSurface(surface_button, NULL, surface_screen, &position_button);
    SDL_FreeSurface(surface_button);
  }

  // Paramètres animation
  char char_animation[50];
  SDL_Surface *surface_animation =  NULL; 
  SDL_Rect position_animation;
  position_animation.x = 50;
  position_animation.y = 50;

  sprintf(char_animation, "%spenguin_%d.bmp", PATH_ANIMATION, num_animation);
  surface_animation = SDL_LoadBMP(char_animation);
  SDL_SetColorKey(surface_animation, SDL_SRCCOLORKEY, SDL_MapRGB(surface_animation->format, 255, 255, 255));
  SDL_BlitSurface(surface_animation, NULL, surface_screen, &position_animation);
  SDL_Flip(surface_screen);

  SDL_FreeSurface(surface_animation);
}


static void display_grid(grid g, SDL_Surface *surface_screen){
  SDL_Surface *surface_tile = NULL;
  SDL_Rect position_tile;
  char name_tile[40];
  for(int i = 0; i < GRID_SIDE; i++){
    for(int j = 0; j < GRID_SIDE; j++){
      position_tile.x = POSITION_TILE_X + i * TILE_SIDE;
      position_tile.y = POSITION_TILE_Y + j * TILE_SIDE;
      sprintf(name_tile, "%s%stile_%d.bmp", PATH_TILE, char_color, (int)get_tile(g, i, j));
      surface_tile = SDL_LoadBMP(name_tile);
      SDL_BlitSurface(surface_tile, NULL, surface_screen, &position_tile);
      SDL_FreeSurface(surface_tile);
    }
  }
}



static void display_score(grid g, SDL_Surface *surface_screen){
  // Parametres affichage score
  TTF_Font *police_text = TTF_OpenFont(PATH_POLICE, 30);
  SDL_Color color_text = {255, 0, 0}; // Couleur rouge
  SDL_Color color_background = {255, 255, 255}; // Couleur blanche
  char char_score[100];
  int position_text_y = 2 * POSITION_BACKGROUND_Y + GRID_SIDE * TILE_SIDE;

  // Ouverture (et si besoin creation) du fichier contenant l'highscore
  FILE* highscore_txt = fopen(PATH_HIGHSCORE, "r+");
  if(highscore_txt == NULL)
    highscore_txt = fopen(PATH_HIGHSCORE, "w");

  // Recupere le pseudo et le highscore sauvegardes precedemment
  char char_highscore[10] = "";
  char char_nickname[10] = "";
  read_line(highscore_txt, char_nickname, char_highscore);
  unsigned long int highscore = strtoul(char_highscore, NULL, 10); // Convertit une chaine de caractere en unsigned long int

  sprintf(char_score, "Score : %lu", grid_score(g));
  display_text(surface_screen, police_text, color_text, color_background, char_score, POSITION_BACKGROUND_Y/2, false);

  // Affiche l'highscore
  if(grid_score(g) >= highscore){
    sprintf(char_highscore, "%lu", grid_score(g));
    write_line(highscore_txt, char_nickname, char_highscore);
    sprintf(char_score, "New Highscore : %s !!", char_highscore);
    display_text(surface_screen, police_text, color_text, color_background, char_score, position_text_y - 5, false);
  }else{
    sprintf(char_score, "Highscore :%s - %s", char_highscore, char_nickname);
    display_text(surface_screen, police_text, color_text, color_background, char_score, position_text_y - 5, false);
  }

  TTF_Font *police_menu = TTF_OpenFont(PATH_POLICE, 25);
  char char_recommencer[30] = "Press ENTER to TRY AGAIN";
  display_text(surface_screen, police_menu, color_text, color_background, char_recommencer, (WINDOW_HEIGHT + 2 * position_text_y)/3 + 5, false);

  char char_giveUp[30] = "or ESC to GIVE UP";
  display_text(surface_screen, police_menu, color_text, color_background, char_giveUp, (2 * WINDOW_HEIGHT + position_text_y)/3 + 5, false);

  // Libere la memoire allouee et ferme le fichier contenant l'Highscore
  TTF_CloseFont(police_text);
  TTF_CloseFont(police_menu);
  fclose(highscore_txt);
}



static void display_gameover(grid g, SDL_Surface *surface_screen, SDL_Surface *surface_background_grid, SDL_Rect position_background_grid, bool *try_again){
  SDL_Color color_text = {255, 255, 255}; // Couleur blanche
  SDL_Color color_background = {0, 0, 0}; // Couleur noire

  char *char_gameover = "GAME OVER";
  TTF_Font *police_gameover = TTF_OpenFont(PATH_POLICE, (GRID_SIDE == 2 ? 30 : 53)); 

  bool end_game = true; // Boolean de la boucle de fin du jeu
  char char_highscore[10]; // Chaine de caractere contenant le highscore
  char char_nickname[10]; // Chaine de caractere contenant le pseudo
  FILE* highscore_txt = fopen(PATH_HIGHSCORE, "r+"); 
  read_line(highscore_txt, char_nickname, char_highscore);
  fclose(highscore_txt);
  unsigned long int highscore = strtoul(char_highscore, NULL, 10); // Convertit une chaine de caractere en unsigned long int

  display_grid(g, surface_screen);

  // Ajout d'un transparent sur la grille (de la couleur du contour de la grille)
  SDL_SetAlpha(surface_background_grid, SDL_SRCALPHA, 125);
  SDL_BlitSurface(surface_background_grid, NULL, surface_screen, &position_background_grid);
  SDL_Flip(surface_screen);

  // Reecriture de l'Highscore si nouvel highscore
  if(grid_score(g) == highscore)
    enter_nickname(g, surface_screen, surface_background_grid, position_background_grid, char_highscore, &end_game, try_again);
  else
    display_text(surface_screen, police_gameover, color_text, color_background, char_gameover, POSITION_TILE_Y + (GRID_SIDE * TILE_SIDE)/2, true);

  SDL_Flip(surface_screen);
  
  // Boucle de fin
  SDL_Event event;
  while(end_game){
    SDL_WaitEvent(&event);
    if(event.type == SDL_QUIT){
      end_game = false;
    }
    else if(event.type == SDL_KEYDOWN){
      switch(event.key.keysym.sym){
      case SDLK_RETURN:
	*try_again = true;
	end_game = false;
	break;
      case SDLK_ESCAPE:
	end_game = false;
	break;
      default:
	break;
      }
    }
  }

  // Libere la memoire allouee
  TTF_CloseFont(police_gameover);
}



static void display_text(SDL_Surface *surface_screen, TTF_Font *police_text, SDL_Color color_text, SDL_Color color_background, char *char_text, int position_height, bool transparence){
  SDL_Surface *surface_text = NULL;
  SDL_Rect position_text;
  surface_text = TTF_RenderText_Shaded(police_text, char_text, color_text, color_background);

  // Met en transparent la couleur du background du texte
  if(transparence)
    SDL_SetColorKey(surface_text, SDL_SRCCOLORKEY, SDL_MapRGB(surface_text->format, 0, 0, 0));

  position_text.x = (surface_screen->w - surface_text->w)/2; // Centre le texte
  position_text.y = position_height - surface_text->h/2;
  SDL_BlitSurface(surface_text, NULL, surface_screen, &position_text);
  
  // Libere la memoire allouee
  SDL_FreeSurface(surface_text);
}



static void enter_nickname(grid g, SDL_Surface *surface_screen, SDL_Surface *surface_background_grid, SDL_Rect position_background_grid, char *char_highscore, bool *end_game, bool *try_again){
  FILE* highscore_txt = fopen(PATH_HIGHSCORE, "r+");
  TTF_Font *police_text = TTF_OpenFont(PATH_POLICE, (GRID_SIDE == 2 ? 20 : 30));
  TTF_Font *police_gameover = TTF_OpenFont(PATH_POLICE, (GRID_SIDE == 2 ? 30 : 53));
  SDL_Color color_text = {255, 255, 255}; // Couleur blanche
  SDL_Color color_background = {0, 0, 0}; // Couleur noire
  char char_nickname[10] = ""; // Chaine de caractere qui contiendra le nouveau pseudo
  char *char_gameover = "GAME OVER";
  char char_display[60] = ""; // Chaine de caractere qui contiendra "char_highscore - char_nickname" 
  char char_tmp[9] = "********"; // Chaine de caractere permettant au joueur de visualiser le nombre de caractere restant pour ecrire son pseudo
  char char_newHighscore[30] = ""; // Chaine de caractere qui contiendra "New Highscore - char_highscore"

  // Position du texte (inherent a GRID_SIDE)
  int position_adjusted = ((GRID_SIDE == 2 ? 2 : 3) * TILE_SIDE)/5;
  int position_y_text = POSITION_TILE_Y + (GRID_SIDE * TILE_SIDE)/2 - position_adjusted * 5/2;

  SDL_Event event;
  int cpt_char = 0; // Nombre de caractere saisi par l'utilisateur
  int nb_max_char = 8; // Nombre maximum de caractere pour le pseudo
  bool re_display = true; // Boolean de reaffichage de l'ecran
  bool enter_new_nickname = true; // Boolean de la boucle de saisi du nouveau pseudo
  SDL_EnableUNICODE(1); // active l'unicode

  // Boucle while permettant de recuperer et de sauvegarder le pseudo saisi par l'utilisateur, dans le fichier hghscore_sdl.txt
  while(enter_new_nickname){
    // Affiche tous les elements lies au Game Over, ainsi que le nouveau pseudo saisi
    if(re_display){
      sprintf(char_display, "%s - %s%s", char_highscore, char_nickname, char_tmp);
      write_line(highscore_txt, char_nickname, char_highscore);

      display_grid(g, surface_screen);

      SDL_SetAlpha(surface_background_grid, SDL_SRCALPHA, 125);
      SDL_BlitSurface(surface_background_grid, NULL, surface_screen, &position_background_grid);
	
      display_text(surface_screen, police_gameover, color_text, color_background, char_gameover, position_y_text + position_adjusted, true);
      sprintf(char_newHighscore, "New Highscore : %s !!", char_highscore);
      display_text(surface_screen, police_text, color_text, color_background, char_newHighscore, position_y_text + 2 * position_adjusted, true);
      display_text(surface_screen, police_text, color_text, color_background, "Enter your nickname :", position_y_text + 3 * position_adjusted,true);

      display_text(surface_screen, police_text, color_text, color_background, char_display, position_y_text + 4 * position_adjusted, true);

      SDL_Flip(surface_screen);
      re_display = false;
    }

    SDL_WaitEvent(&event);
    if(event.type == SDL_QUIT){
      enter_new_nickname = false;
      *end_game = false;
    }
    else if(event.type == SDL_KEYDOWN){
      switch(event.key.keysym.sym){
      // Quitter la partie
      case SDLK_ESCAPE:
	enter_new_nickname = false;
	*end_game = false;
	break;
      case SDLK_RETURN: // Touche "entree"
	enter_new_nickname = false;
	*end_game = false;
	*try_again = true;
	break;
      case SDLK_BACKSPACE: // Touche "supprimer"
	if(cpt_char > 0){
	  char_nickname[strlen(char_nickname) - 1] = '\0';
	  char_tmp[nb_max_char - cpt_char] = '*';
	  re_display = true;
	  cpt_char -= 1;
	}
	break;
      default:
	if(nb_max_char > cpt_char){
	  // Si la touche est une lettre, un chiffre, ou un symbole
	  if(event.key.keysym.unicode >= 32 && event.key.keysym.unicode <= 126){
	    sprintf(char_nickname, "%s%c", char_nickname, (char)event.key.keysym.unicode);
	    char_tmp[nb_max_char - cpt_char - 1] = '\0';
	    re_display = true;
	    cpt_char += 1;
	  }
	}
	break;
      }
    }
    // S'il y a des deplacements de souris, les ignorer
    else if(event.type == SDL_MOUSEMOTION)
      continue;
  }

  SDL_EnableUNICODE(0); // desactive l'unicode

  // Libere la memoire allouee et ferme le fichier stockant le Highscore
  TTF_CloseFont(police_text);
  TTF_CloseFont(police_gameover);
  fclose(highscore_txt);
}



static void read_line(FILE *fichier, char *char_nickname, char *char_highscore){
  rewind(fichier); // Revient au debut du fichier

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
  rewind(fichier); // Revient au debut du fichier

  // Si la chaine passee en parametre est vide, on ecrit "Anyone" par defaut dans le pseudo
  if(strlen(char_nickname) == 0){
    char_nickname = "Anyone";
  }

  fprintf(fichier, "%-10s%-10s", char_nickname, char_highscore);
}
