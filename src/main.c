#include <stdio.h>
#include "commun.h"
#include "game.h"
#include "replay.h"
#include "stats.h"
#include "config.h"
#include "ui.h" // For printLogo if needed (called by afficherMenu)
#include <unistd.h>

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
    // Color IDs: 1=Red, 3=Yellow
    ParametresJeu parametres = {7, "rond", 1, 3, 30, 1, 2, 2, 'X', 'O'};
    // modeJeu default 2 (PvC), difficulty 2 (Medium), symbols X/O
    
    // Attempt load config
    if (!chargerConfig(&parametres)) {
         // First time
         clearScreen();
         printLogo();
         printCentered("Bienvenue ! C'est votre premiere partie.");
         printCentered("Configuration initiale...");
         sleep(2);
         gererParametres(&parametres);
         sauvegarderConfig(&parametres);
    }

    do {
        afficherMenu();
        scanf("%d", &choix);
        getchar(); // consommer '\n'

        switch (choix) {
            case 1:
                if (aDesSauvegardes()) {
                    int subChoix;
                    do {
                        clearScreen();
                        printCentered("1. Nouvelle Partie");
                        printCentered("2. Charger une partie");
                        printCentered("0. Retour");
                        printCenteredPrompt("Choix : ");
                        scanf("%d", &subChoix);
                        getchar();
                        if (subChoix == 1) {
                            jouerPartie(&parametres, NULL);
                            break;
                        } else if (subChoix == 2) {
                            chargerPartie(&parametres);
                            break;
                        } else if (subChoix == 0) {
                            break;
                        } else {
                            printf("Option invalide.\n");
                        }
                    } while (subChoix != 0);
                } else {
                    jouerPartie(&parametres, NULL);
                }
                break;
            case 2:
                gererParametres(&parametres);
                sauvegarderConfig(&parametres); // Auto-save on change
                break;
            case 3:
                // Call the new replay viewer logic
                // The prototype for afficherReplays in main was just a placeholder print
                // We'll use the one from replay.h or call visionnerReplay directly
                // But we need to include replay.h
                visionnerReplay();
                break;
            case 4:
                afficherStats();
                break;
            case 5:
                printf("Quitter le jeu. Au revoir !\n");
                sauvegarderConfig(&parametres);
                break;
            default:
                printf("Choix invalide, veuillez reessayer.\n");
        }
    } while (choix != 5);

    return 0;
}
