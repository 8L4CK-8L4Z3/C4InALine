#ifndef COMMUN_H
#define COMMUN_H

#define MAX_SAVED_GAMES 10

typedef struct {
    int tailleGrille;
    char formePions[20];
    char couleurPions[20];
    int tempsLimite; // en secondes
    int autosave;    // 0 ou 1
} ParametresJeu;

typedef struct {
    char nomPartie[50];
    ParametresJeu parametres;
    // ajouter plus d’infos de la partie ici si besoin
} PartieSauvegardee;

// Prototypes fonctions
void afficherMenu();
void afficherReplays();
void gererParametres(ParametresJeu *params);
void afficherSauvegardes();
void chargerPartie();
void sauvegarderPartie();

#endif
