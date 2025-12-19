#ifndef COMMUN_H
#define COMMUN_H

#define MAX_SAVED_GAMES 10
#define MAX_ROWS 20
#define MAX_COLS 20
#define MAX_MOVES_TOTAL (MAX_ROWS * MAX_COLS)

typedef struct {
    int tailleGrille; // Taille carrée par défaut
    char formePions[20];
    char couleurPions[20];
    int tempsLimite; // en secondes
    int autosave;    // 0 ou 1
    int modeJeu;     // 1: PvP, 2: PvC
    char symboleJ1;
    char symboleJ2;
} ParametresJeu;

typedef struct {
    char nomPartie[50];
    ParametresJeu parametres;
    char grille[MAX_ROWS][MAX_COLS];
    int rows;
    int cols;
    int joueurCourant;
    int moves[MAX_MOVES_TOTAL];
    int moveCount;
} PartieSauvegardee;

// Prototypes fonctions
void afficherMenu();
void afficherReplays();
void gererParametres(ParametresJeu *params);
void afficherSauvegardes();
void chargerPartie(ParametresJeu *currentParams);
void sauvegarderPartie(char **grille, int rows, int cols, int joueurCourant, ParametresJeu *params, int *moves, int moveCount);
void sauvegarderPartieSilencieuse(char **grille, int rows, int cols, int joueurCourant, ParametresJeu *params, int *moves, int moveCount);

#endif
