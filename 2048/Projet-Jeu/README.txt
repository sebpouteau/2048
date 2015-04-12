        ### PROJET 2048 ###
(Licence 2 d'informatique - Année 2015)


Réalisé par:
  Ysabelle Emery,
  Jimmy Gouraud,
  Yordan Kirov,
  Sébastien Pouteau.

/* ================================= 
                LE JEU
   ================================= */

Aller dans Projet-Jeu/

/!\ WARNING /!\
Vérifiez au préalable que vous avez bien les bibliothèques ncurses et SDL d'installées.
Ainsi que deux bibliothèques tierces (SDL_image et SDL_ttf), sinon il y aura un échec lors de la compilation.


Compilation du Projet 2048 grace à la commande make :
  make 

Pour vérifier que toutes les fonctions marchent, lancer le test avec :
 make check

Les librairies créées sont placées dans lib/
Les exécutables sont placés dans bin/	

/* ================================= 
                L'IA
   ================================= */

Compilation des IA grace à la commande make :
  make 

les exécutables sont dans bin/  (IA-repetition-fast/low "nombre de repetition"  && IA-graphique-fast/low)
Les librairies créées sont placées dans lib/

Les bibliothèques dynamiques créées sont placées à la racine
Une bibliothèque dynamique est crée par la commande make, si vous souhaitez l'utiliser vous devez ajouter dans votre ./bash la commande:
    export LD_LIBRARY_PATH=../lib:$LD_LIBRARY_PATH
