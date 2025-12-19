#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>
#include <time.h>
#include "game.h"
#include "grid.h"
#include "ai.h"
#include "commun.h"
#include "replay.h"
#include "stats.h"
#include "ui.h"
#include "input.h"
#include <string.h>

// Helper to clear input buffer
void viderBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// fonction pour jouer un tour
// Returns: 1 if success, 0 if invalid move/retry needed, -99 if save requested
int jouerTour(int joueur, char **grille, int rows, int cols, ParametresJeu *params, int *playedCol) {
    char symbole = (joueur == 1) ? params->symboleJ1 : params->symboleJ2;
    int col = -1;

    // AI Check
    if (params->modeJeu == 2 && joueur == 2) {
        printCentered("L'ordinateur reflechit...");
        sleep(1);
        col = calculerCoupOrdi(grille, rows, cols, params->symboleJ2, params->symboleJ1, params->difficulty);
        char buf[100];
        snprintf(buf, sizeof(buf), "L'ordinateur joue en colonne %d", col);
        printCentered(buf);
    } else {
        // Human player with arrow keys
        int selectedCol = 0;
        configureTerminal();
        
        // Loop for input
        time_t start = time(NULL);
        
        while(1) {
            // Check timeout
            if (difftime(time(NULL), start) >= params->tempsLimite) {
                restoreTerminal();
                printCentered("\nTemps ecoule ! Vous avez perdu.");
                col = -2; // Code pour défaite par temps
                break;
            }

            // Display cursor
            clearScreen();

            // Render cursor line centered
            // We need to match afficherGrille centering logic.
            int termWidth = getTerminalWidth();
            int gridWidth = 2 + 2 * cols;
            int padding = (termWidth - gridWidth) / 2;
            if (padding < 0) padding = 0;
            
            // Print cursor
            printf("\n");
            for(int k=0; k<padding; k++) printf(" ");
            printf("  "); // Offset for row numbers
            for(int c=0; c<cols; c++) {
                if (c == selectedCol) printf("v "); // Cursor
                else printf("  ");
            }
            printf("\n");

            afficherGrille(grille, rows, cols, params);
            
            char prompt[200];
            snprintf(prompt, sizeof(prompt), "Joueur %d (%c) - Fleches pour bouger, Entree pour valider, 'S' pour sauver", joueur, symbole);
            printCentered(prompt);
            printf("\n"); // Flush

            // Check input with select (for timeout precision, though time(NULL) check above handles it coarsely)
            // But readKey is blocking? No, we set VMIN=0. But wait, VMIN=0 means non-blocking.
            // If VMIN=0, read returns 0 immediately if no input.
            // We should use select to wait for input OR timeout.
            
            fd_set set;
            struct timeval timeout;
            FD_ZERO(&set);
            FD_SET(STDIN_FILENO, &set);
            timeout.tv_sec = 0;
            timeout.tv_usec = 100000; // 0.1 sec refresh for timer check

            int rv = select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout);
            
            if (rv > 0) {
                int key = readKey();
                if (key == KEY_RIGHT) {
                    selectedCol = (selectedCol + 1) % cols;
                } else if (key == KEY_LEFT) {
                    selectedCol = (selectedCol - 1 + cols) % cols;
                } else if (key == KEY_ENTER) {
                    col = selectedCol;
                    restoreTerminal();
                    break;
                } else if (key == 's' || key == 'S') {
                    col = -99;
                    restoreTerminal();
                    break;
                }
            }
        }
        
        restoreTerminal();
    }

    if (col == -99) return -99;
    if (col == -2) return -2;

    if (!insererPion(grille, col, symbole, rows, cols)) {
        printCentered(" Colonne invalide ou pleine ! Reessayez.");
        // If invalid, we need to loop again?
        // Recursive call? Or return 0. 
        // Returning 0 causes loop in jouerPartie.
        sleep(1);
        return 0;
    }

    if (playedCol) *playedCol = col;
    return 1;
}

// fonction pour vérifier la victoire d'un joueur
int verifierVictoire(char **grille, int rows, int cols, char symbole) {
    return verifierGrille(grille, rows, cols, symbole);
}

// fonction pour vérifier si la grille est pleine
static int grillePleine(char **grille, int rows, int cols) {
    for (int j = 0; j < cols; j++) {
        if (!colonnePleine(grille, j, rows))
            return 0;
    }
    return 1;
}

// fonction pour jouer une partie complète
void jouerPartie(ParametresJeu *params, PartieSauvegardee *saveToResume) {
    int rows = params->tailleGrille;
    int cols = params->tailleGrille;
    char **grille = creerGrille(rows, cols);
    int joueur = 1;

    // Resume logic
    if (saveToResume != NULL) {
        // Copy grid from save
        for(int i=0; i<rows; i++) {
            for(int j=0; j<cols; j++) {
                grille[i][j] = saveToResume->grille[i][j];
            }
        }
        joueur = saveToResume->joueurCourant;
        printf("Partie reprise. Joueur %d a vous !\n", joueur);
    }

    time_t startTime = time(NULL);
    char symbole;

    // Replay recording
    int moves[MAX_MOVES_TOTAL];
    int moveCount = 0;

    // Resume moves
    if (saveToResume != NULL) {
        if (saveToResume->moveCount <= MAX_MOVES_TOTAL) {
             memcpy(moves, saveToResume->moves, saveToResume->moveCount * sizeof(int));
             moveCount = saveToResume->moveCount;
        }
    }

    while (1) {
        clearScreen();
        afficherGrille(grille, rows, cols, params);
        
        // Autosave check
        if (params->autosave) {
             sauvegarderPartieSilencieuse(grille, rows, cols, joueur, params, moves, moveCount);
        }
        
        int result;
        int playedCol = -1;
        do {
            result = jouerTour(joueur, grille, rows, cols, params, &playedCol);
            
            if (result == -99) {
                sauvegarderPartie(grille, rows, cols, joueur, params, moves, moveCount);
                libererGrille(grille, rows);
                return; // Exit game
            }
            if (result == -2) {
                break; // Timeout loss handled below
            }
        } while (result == 0);
        
        if (result == -2) {
            // Handle timeout loss (current player lost)
            int winner = (joueur == 1) ? 2 : 1;
            char winnerSym = (winner == 1) ? params->symboleJ1 : params->symboleJ2;

            clearScreen();
            afficherGrille(grille, rows, cols, params);
            char buf[200];
            snprintf(buf, sizeof(buf), "\n\033[1;32m*** VICTOIRE ! Joueur %d (%c) a gagne par forfait (temps) ! ***\033[0m", winner, winnerSym);
            printCentered(buf);
            sauvegarderReplay(moves, moveCount, rows, cols, params, winner);
            mettreAJourStats(winner, (int)difftime(time(NULL), startTime));
            break;
        }

        if (moveCount < MAX_MOVES_TOTAL) {
            moves[moveCount++] = playedCol;
        }

        symbole = (joueur == 1) ? params->symboleJ1 : params->symboleJ2;

        // vérifier victoire
        if (verifierVictoire(grille, rows, cols, symbole)) {
            clearScreen();
            afficherGrille(grille, rows, cols, params);
            char buf[200];
            snprintf(buf, sizeof(buf), "\n\033[1;32m*** VICTOIRE ! Joueur %d (%c) a gagne ! ***\033[0m", joueur, symbole);
            printCentered(buf);
            sauvegarderReplay(moves, moveCount, rows, cols, params, joueur);
            mettreAJourStats(joueur, (int)difftime(time(NULL), startTime));
            break;
        }

        // vérifier match nul
        if (grillePleine(grille, rows, cols)) {
            clearScreen();
            afficherGrille(grille, rows, cols, params);
            printCentered("\n\033[1;33m*** MATCH NUL ! La grille est pleine. ***\033[0m");
            sauvegarderReplay(moves, moveCount, rows, cols, params, 0);
            mettreAJourStats(0, (int)difftime(time(NULL), startTime));
            break;
        }

        // changer de joueur
        joueur = (joueur == 1) ? 2 : 1;
    }

    libererGrille(grille, rows);
    printCentered("Appuyez sur Entree pour continuer...");
    getchar(); // Wait for user
    getchar();
}
