#include <stdio.h>
#include <stdlib.h> // for exit
#include "commun.h"
#include "game.h"
#include "replay.h"
#include "stats.h"
#include "config.h"
#include "input.h"
#include "ui.h"

// Prototypes
int afficherMenuTUI(); // Returns choice
void gererParametres(ParametresJeu *params);
void afficherSauvegardes();
void chargerPartie(ParametresJeu *params);
void sauvegarderPartie(char **grille, int rows, int cols, int joueurCourant, ParametresJeu *params, int *moves, int moveCount);

#include <time.h>

int main() {
    srand(time(NULL)); // Init random seed once

    // Defaults
    // Color IDs: 1=Red, 3=Yellow
    ParametresJeu parametres = {7, "rond", 1, 3, 30, 1, 2, 'X', 'O'}; 
    
    // Load config or force wizard
    if (!loadGlobalConfig(&parametres)) {
        clearScreen();
        printLogo();
        printf("\n\n\033[1;33mBienvenue ! C'est votre premiere fois.\033[0m\n");
        printf("Veuillez configurer le jeu.\n");
        printf("Appuyez sur une touche...\n");
        // Need basic blocking read if raw mode not yet enabled?
        // Let's use standard getchar
        getchar();
        gererParametres(&parametres);
        saveGlobalConfig(&parametres);
    }

    // Enter TUI Loop
    enableRawMode();

    int choix;
    do {
        choix = afficherMenuTUI();

        switch (choix) {
            case 1: // Jouer
                // Disable raw mode for the game itself if it uses old logic? 
                // OR we refactor game to use raw mode. 
                // The plan says we refactor game. So keep raw mode.
                jouerPartie(&parametres, NULL);
                // Ensure raw mode is enabled when returning (jouerPartie might mess with it?)
                // Actually jouerPartie uses select/scanf. Scanf with raw mode is weird.
                // We should update jouerPartie to use readKey.
                break;
            case 2: // Parametres
                disableRawMode(); // gererParametres currently uses scanf
                gererParametres(&parametres);
                saveGlobalConfig(&parametres);
                enableRawMode();
                break;
            case 3: // Charger
                disableRawMode(); // Uses scanf
                chargerPartie(&parametres);
                enableRawMode();
                break;
            case 4: // Rejouer
                disableRawMode(); // replay viewer uses scanf
                visionnerReplay();
                enableRawMode();
                break;
            case 5: // Stats
                disableRawMode();
                afficherStats();
                printf("\nAppuyez sur Entree pour revenir...");
                getchar(); getchar(); // Flush/Wait
                enableRawMode();
                break;
            case 6: // Quitter
                disableRawMode();
                printf("Au revoir !\n");
                break;
        }
    } while (choix != 6);

    return 0;
}
