# Documentation des Fonctions

Ce document liste toutes les fonctions du dépôt, en décrivant leurs entrées, sorties et rôles.

## src/ai.c

### `calculerCoupOrdi`
*   **Entrée** : `char **grille` (grille de jeu), `int rows`, `int cols`, `char symboleOrdi`, `char symboleJoueur`, `int difficulty`
*   **Sortie** : `int` (index de la colonne)
*   **Fonction** : Calcule le meilleur coup pour l'ordinateur en fonction du niveau de difficulté. Niveau 1 est aléatoire, Niveau 2 bloque/gagne immédiatement, Niveau 3 utilise Minimax.

### `simulateWin` (statique)
*   **Entrée** : `char **grille`, `int col`, `int rows`, `int cols`, `char symbol`
*   **Sortie** : `int` (1 si victoire, 0 sinon)
*   **Fonction** : Place temporairement un pion pour vérifier si cela mène à une victoire, puis annule le changement.

### `minimax` (statique)
*   **Entrée** : `char **grille`, `int depth`, `int alpha`, `int beta`, `int maximizingPlayer`, `int rows`, `int cols`, `char symboleOrdi`, `char symboleJoueur`
*   **Sortie** : `int` (score)
*   **Fonction** : Algorithme récursif Minimax avec élagage Alpha-Beta pour évaluer les états du plateau.

## src/autosave.c

### `sauvegarderPartieSilencieuse`
*   **Entrée** : `char **grille`, `int rows`, `int cols`, `int joueurCourant`, `ParametresJeu *params`, `int *moves`, `int moveCount`
*   **Sortie** : `void`
*   **Fonction** : Sauvegarde l'état actuel de la partie dans `autosave.dat` sans interaction utilisateur ni affichage console.

## src/config.c

### `chargerConfig`
*   **Entrée** : `ParametresJeu *params`
*   **Sortie** : `int` (1 si succès, 0 si échec)
*   **Fonction** : Charge la configuration du jeu depuis `config.dat` dans la structure fournie.

### `sauvegarderConfig`
*   **Entrée** : `ParametresJeu *params`
*   **Sortie** : `void`
*   **Fonction** : Sauvegarde la configuration actuelle du jeu dans `config.dat`.

## src/game.c

### `jouerTour`
*   **Entrée** : `int joueur`, `char **grille`, `int rows`, `int cols`, `ParametresJeu *params`, `int *playedCol`
*   **Sortie** : `int` (1 succès, 0 invalide/réessayer, -99 sauvegarde demandée, -2 temps écoulé)
*   **Fonction** : Gère un tour pour un joueur (humain ou IA). Gère l'entrée, la validation du coup, les délais et l'affichage du curseur.

### `verifierVictoire`
*   **Entrée** : `char **grille`, `int rows`, `int cols`, `char symbole`
*   **Sortie** : `int` (1 si victoire, 0 sinon)
*   **Fonction** : Enveloppe pour `verifierGrille` afin de vérifier si le symbole spécifié a gagné.

### `grillePleine` (statique)
*   **Entrée** : `char **grille`, `int rows`, `int cols`
*   **Sortie** : `int` (1 si pleine, 0 sinon)
*   **Fonction** : Vérifie si la grille entière est pleine (condition de match nul).

### `jouerPartie`
*   **Entrée** : `ParametresJeu *params`, `PartieSauvegardee *saveToResume`
*   **Sortie** : `void`
*   **Fonction** : Gère la boucle principale du jeu, incluant l'alternance des tours, la détection de victoire/nul, la sauvegarde automatique et l'enregistrement du replay. Peut reprendre une partie sauvegardée.

### `viderBuffer` (statique)
*   **Entrée** : Aucune
*   **Sortie** : `void`
*   **Fonction** : Vide le tampon d'entrée standard.

## src/grid.c

### `creerGrille`
*   **Entrée** : `int rows`, `int cols`
*   **Sortie** : `char**` (pointeur vers la grille 2D)
*   **Fonction** : Alloue la mémoire pour une grille de caractères 2D et l'initialise avec des cellules vides (`.`).

### `libererGrille`
*   **Entrée** : `char **grille`, `int rows`
*   **Sortie** : `void`
*   **Fonction** : Libère la mémoire allouée pour la grille.

### `getAnsiColor` (statique)
*   **Entrée** : `int c` (ID couleur)
*   **Sortie** : `const char*` (Code d'échappement ANSI)
*   **Fonction** : Mappe un ID de couleur entier vers une chaîne de couleur ANSI.

### `afficherGrille`
*   **Entrée** : `char **grille`, `int rows`, `int cols`, `ParametresJeu *params`
*   **Sortie** : `void`
*   **Fonction** : Affiche la grille de jeu dans la console avec le formatage, les couleurs et le centrage appropriés.

### `colonnePleine`
*   **Entrée** : `char **grille`, `int col`, `int rows`
*   **Sortie** : `int` (1 si pleine, 0 sinon)
*   **Fonction** : Vérifie si une colonne spécifique de la grille est pleine.

### `insererPion`
*   **Entrée** : `char **grille`, `int col`, `char symbole`, `int rows`, `int cols`
*   **Sortie** : `int` (1 si succès, 0 si colonne invalide ou pleine)
*   **Fonction** : Insère un pion dans l'emplacement libre le plus bas de la colonne spécifiée.

### `verifierGrille`
*   **Entrée** : `char **grille`, `int rows`, `int cols`, `char symbole`
*   **Sortie** : `int` (1 si victoire trouvée, 0 sinon)
*   **Fonction** : Vérifie la présence de 4 symboles connectés horizontalement, verticalement ou en diagonale.

## src/input.c

### `configureTerminal`
*   **Entrée** : Aucune
*   **Sortie** : `void`
*   **Fonction** : Configure le terminal en mode brut (désactive l'écho et le mode canonique) pour la lecture immédiate des touches.

### `restoreTerminal`
*   **Entrée** : Aucune
*   **Sortie** : `void`
*   **Fonction** : Restaure le terminal à ses paramètres d'origine.

### `readKey`
*   **Entrée** : Aucune
*   **Sortie** : `int` (code touche)
*   **Fonction** : Lit une touche depuis stdin, gérant les séquences d'échappement pour les flèches directionnelles.

## src/main.c

### `main`
*   **Entrée** : Aucune
*   **Sortie** : `int` (statut de sortie)
*   **Fonction** : Point d'entrée du programme. Initialise la configuration, gère la boucle du menu principal et répartit les actions selon le choix de l'utilisateur.

## src/menu.c

### `menuSelection`
*   **Entrée** : `const char *titre`, `const char *options[]`, `int nbOptions`
*   **Sortie** : `int` (index de l'option sélectionnée, base 1)
*   **Fonction** : Affiche un menu vertical interactif avec navigation par flèches et retourne la sélection de l'utilisateur.

### `afficherMenuInteractif`
*   **Entrée** : Aucune
*   **Sortie** : `int` (option sélectionnée)
*   **Fonction** : Affiche le menu principal en utilisant `menuSelection`.

### `afficherMenu`
*   **Entrée** : Aucune
*   **Sortie** : `void`
*   **Fonction** : Fonction héritée pour afficher le menu (actuellement inutilisée/placeholder).

### `afficherReplays`
*   **Entrée** : Aucune
*   **Sortie** : `void`
*   **Fonction** : Fonction placeholder pour afficher les replays (logique réelle dans `replay.c`).

## src/replay.c

### `chargerReplaysDepuisFichier`
*   **Entrée** : Aucune
*   **Sortie** : `void`
*   **Fonction** : Charge les données de replay depuis `replays.dat` dans la structure mémoire globale.

### `ecrireReplaysFichier`
*   **Entrée** : Aucune
*   **Sortie** : `void`
*   **Fonction** : Écrit les données globales de replay dans `replays.dat`.

### `sauvegarderReplay`
*   **Entrée** : `int *moves`, `int moveCount`, `int rows`, `int cols`, `ParametresJeu *params`, `int winner`
*   **Sortie** : `void`
*   **Fonction** : Crée une nouvelle entrée de replay à partir de l'historique de la partie et la sauvegarde dans le fichier. Gère la rotation si le max de replays est atteint.

### `afficherListeReplays`
*   **Entrée** : Aucune
*   **Sortie** : `void`
*   **Fonction** : Liste les replays disponibles à l'écran.

### `visionnerReplay`
*   **Entrée** : Aucune
*   **Sortie** : `void`
*   **Fonction** : Fonction interactive pour sélectionner et regarder un replay. Rejoue la partie coup par coup.

## src/save.c

### `chargerSauvegardesDepuisFichier`
*   **Entrée** : Aucune
*   **Sortie** : `void`
*   **Fonction** : Charge les parties sauvegardées depuis `sauvegardes.dat` en mémoire.

### `aDesSauvegardes`
*   **Entrée** : Aucune
*   **Sortie** : `int` (1 si des sauvegardes existent, 0 sinon)
*   **Fonction** : Vérifie s'il y a des parties sauvegardées disponibles.

### `ecrireSauvegardesFichier`
*   **Entrée** : Aucune
*   **Sortie** : `void`
*   **Fonction** : Écrit les parties sauvegardées en mémoire dans `sauvegardes.dat`.

### `afficherSauvegardes`
*   **Entrée** : Aucune
*   **Sortie** : `void`
*   **Fonction** : Affiche une liste des parties sauvegardées (utilisé pour le débogage ou liste simple).

### `chargerPartie`
*   **Entrée** : `ParametresJeu *currentParams`
*   **Sortie** : `void`
*   **Fonction** : Fonction interactive pour sélectionner une partie sauvegardée et la reprendre. Met à jour les paramètres actuels.

### `sauvegarderPartie`
*   **Entrée** : `char **grille`, `int rows`, `int cols`, `int joueurCourant`, `ParametresJeu *params`, `int *moves`, `int moveCount`
*   **Sortie** : `void`
*   **Fonction** : Demande interactivement un nom de sauvegarde et sauvegarde l'état actuel de la partie dans la liste des sauvegardes.

## src/settings.c

### `choisirCouleur`
*   **Entrée** : `int *colorRef`, `int playerNum`
*   **Sortie** : `void`
*   **Fonction** : Menu interactif pour sélectionner une couleur pour un joueur. Met à jour la variable de couleur référencée.

### `getColorName`
*   **Entrée** : `int c`
*   **Sortie** : `const char*`
*   **Fonction** : Retourne le nom de la chaîne correspondant à un ID de couleur.

### `gererParametres`
*   **Entrée** : `ParametresJeu *params`
*   **Sortie** : `void`
*   **Fonction** : Menu interactif pour voir et modifier les paramètres du jeu (taille grille, symboles, couleurs, temps limite, mode, difficulté).

## src/stats.c

### `mettreAJourStats`
*   **Entrée** : `int winner`, `int tempsDuree`
*   **Sortie** : `void`
*   **Fonction** : Met à jour les statistiques globales (parties jouées, victoires, temps) dans `stats.dat`.

### `afficherStats`
*   **Entrée** : Aucune
*   **Sortie** : `void`
*   **Fonction** : Lit et affiche les statistiques globales.

## src/ui.c

### `clearScreen`
*   **Entrée** : Aucune
*   **Sortie** : `void`
*   **Fonction** : Efface l'écran du terminal en utilisant les codes d'échappement ANSI.

### `getTerminalWidth`
*   **Entrée** : Aucune
*   **Sortie** : `int` (nombre de colonnes)
*   **Fonction** : Retourne la largeur actuelle de la fenêtre du terminal.

### `getTerminalHeight`
*   **Entrée** : Aucune
*   **Sortie** : `int` (nombre de lignes)
*   **Fonction** : Retourne la hauteur actuelle de la fenêtre du terminal.

### `visibleLength` (statique)
*   **Entrée** : `const char *s`
*   **Sortie** : `int`
*   **Fonction** : Calcule la longueur visible d'une chaîne, en ignorant les codes d'échappement ANSI.

### `printCentered`
*   **Entrée** : `const char *format`, ...
*   **Sortie** : `void`
*   **Fonction** : Affiche une chaîne formatée centrée horizontalement dans le terminal. Gère les chaînes multi-lignes.

### `printCenteredPrompt`
*   **Entrée** : `const char *format`, ...
*   **Sortie** : `void`
*   **Fonction** : Affiche une chaîne formatée centrée horizontalement sans saut de ligne final (utile pour les invites).

### `printLogo`
*   **Entrée** : Aucune
*   **Sortie** : `void`
*   **Fonction** : Affiche le logo du jeu centré et en couleur.
