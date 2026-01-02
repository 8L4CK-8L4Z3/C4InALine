#include <stdio.h>
#include "commun.h"
#include "game.h"
#include "replay.h"
#include "stats.h"
#include "config.h"
#include "ui.h" // For printLogo if needed (called by afficherMenu)
#ifdef _WIN32
#include <windows.h>
#define sleep(x) Sleep((x)*1000)
#else
#include <unistd.h>
#endif

// Prototypes
void afficherMenu();
void gererParametres(ParametresJeu *params);
void afficherSauvegardes();
void chargerPartie(ParametresJeu *params);
void sauvegarderPartie(char **grille, int rows, int cols, int joueurCourant, ParametresJeu *params, int *moves, int moveCount);
void afficherReplays();

#include <time.h>

int main() {
#ifdef _WIN32
    setupConsole();
#endif
    srand(time(NULL)); // Init random seed once

    int choix;
    // Defaults
    // Color IDs: 1=Red, 3=Yellow
    ParametresJeu parametres = {
        .tailleGrille = 7,
        .formePions = "rond",
        .colorJ1 = 1,
        .colorJ2 = 3,
        .tempsLimite = 30,
        .autosave = 1,
        .modeJeu = 2,
        .difficulty = 2,
        .symboleJ1 = 'X',
        .symboleJ2 = 'O'
    }; 
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
        choix = afficherMenuInteractif();

        switch (choix) {
            case 1:
                if (aDesSauvegardes()) {
                    const char *subOptions[] = {"Nouvelle Partie", "Charger une partie", "Retour"};
                    int subChoix;
                    do {
                        subChoix = menuSelection("Jouer", subOptions, 3);
                        // menuSelection returns 1-based index
                        // 1: Nouvelle, 2: Charger, 3: Retour
                        
                        if (subChoix == 1) {
                            jouerPartie(&parametres, NULL);
                            break;
                        } else if (subChoix == 2) {
                            chargerPartie(&parametres);
                            break;
                        } else if (subChoix == 3) {
                            break; // Back to main loop
                        }
                    } while (subChoix != 3);
                } else {
                    jouerPartie(&parametres, NULL);
                }
                break;
            case 2:
                gererParametres(&parametres);
                sauvegarderConfig(&parametres); // Auto-save on change
                break;
            case 3:
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
