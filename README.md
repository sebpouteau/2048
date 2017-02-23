# PROJET 2048
(Licence 2 Informatique - Année 2015)


Réalisé par:
- Ysabelle Emery,
- Jimmy Gouraud,
- Yordan Kirov,
- Sébastien Pouteau.

## LE JEU

Aller dans Projet-Jeu/

/!\ WARNING /!\
Vérifiez au préalable que vous avez bien les bibliothèques ncurses et SDL d'installées.
Ainsi que deux bibliothèques tierces (SDL_image et SDL_ttf).


Compilation du Projet 2048 grace à la commande make :
  make

Pour vérifier que toutes les fonctions fonctionnent, lancer le test avec :
 make check

Les librairies créées sont placées dans lib/
Les exécutables sont placés dans bin/	(jeu_ncurses ou jeu_sdl)


## L'IA

Compilation des IA grace à la commande make :
  make

Les librairies créées sont placées dans lib/
Les exécutables sont dans bin/  (IA-repetition-fast/low "nombre de repetition"  && IA-graphique-fast/low)



## DOCUMENTATION

Commande pour obtenir la Documentation:
  make doc

La documentation en html:
   - Aller dans le dossier html/
   - Ouvrir le fichier index.html

La documentation en pdf:
   - Aller dans le dossier latex/
   - 'make'
   - Ouvrir le fichier refman.pdf
