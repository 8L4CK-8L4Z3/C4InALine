#include <stdio.h>
#include "commun.h"
#include "game.h"
#include "replay.h"
#include "stats.h"

// Prototypes
void afficherMenu();
void gererParametres(ParametresJeu *params);
void afficherSauvegardes();
void chargerPartie(ParametresJeu *params);
void sauvegarderPartie(char **grille, int rows, int cols, int joueurCourant, ParametresJeu *params, int *moves, int moveCount);
void afficherReplays();

#include <time.h>

int main() {
    srand(time(NULL)); // Init random seed once

    int choix;
    // Defaults
    ParametresJeu parametres = {7, "rond", "rouge", 30, 1, 2, 'X', 'O'}; 
    // modeJeu default 2 (PvC), symbols X/O

    do {
        afficherMenu();
        scanf("%d", &choix);
        getchar(); // consommer '\n'

        switch (choix) {
            case 1:
                jouerPartie(&parametres, NULL);
                break;
            case 2:
                gererParametres(&parametres);
                break;
            case 3: {
                int subChoix;
                do {
                    printf("\n1. Charger une partie\n2. (Sauvegarde en jeu)\n0. Retour\nChoix : ");
                    scanf("%d", &subChoix);
                    getchar();
                    switch(subChoix) {
                        case 1:
                            chargerPartie(&parametres);
                            break;
                        case 2:
                            printf("Lancez une partie pour sauvegarder.\n");
                            break;
                        case 0:
                            break;
                        default:
                            printf("Option invalide.\n");
                    }
                } while (subChoix != 0);
                break;
            }
            case 4:
                // Call the new replay viewer logic
                // The prototype for afficherReplays in main was just a placeholder print
                // We'll use the one from replay.h or call visionnerReplay directly
                // But we need to include replay.h
                visionnerReplay();
                break;
            case 5:
                afficherStats();
                break;
            case 6:
                printf("Quitter le jeu. Au revoir !\n");
                break;
            default:
                printf("Choix invalide, veuillez reessayer.\n");
        }
    } while (choix != 6);

    return 0;
}
