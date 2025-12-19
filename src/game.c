#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "game.h"
#include "grid.h"
#include "ai.h"
#include "commun.h"
#include "replay.h"
#include "stats.h"
#include "input.h"
#include "ui.h"

// Helper to clear input buffer
void viderBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// fonction pour jouer un tour
// Returns: 1 if success, 0 if retry, -99 if save requested
// Uses TUI arrow keys for human player
int jouerTour(int joueur, char **grille, int rows, int cols, ParametresJeu *params, int *playedCol) {
    char symbole = (joueur == 1) ? params->symboleJ1 : params->symboleJ2;
    static int cursorCol = 0; // State for cursor
    if (cursorCol >= cols) cursorCol = cols - 1;

    // AI Check
    if (params->modeJeu == 2 && joueur == 2) {
        printf("L'ordinateur reflechit...\n");
        int col = calculerCoupOrdi(grille, rows, cols, params->symboleJ2, params->symboleJ1);
        printf("L'ordinateur joue en colonne %d\n", col);
        sleep(1); // Short delay for UX
        if (!insererPion(grille, col, symbole, rows, cols)) return 0;
        if (playedCol) *playedCol = col;
        return 1;
    } 

    // Human Player TUI
    // Initial draw
    clearScreen();
    printf("\033[1;36m=== Tour Joueur %d (%c) ===\033[0m\n", joueur, symbole);
    if (params->modeJeu == 1 || joueur == 1) {
        printf("Utilisez GAUCHE/DROITE pour viser, ENTREE pour jouer, ESC pour sauver/quitter.\n");
    }
    afficherGrilleTUI(grille, rows, cols, params, cursorCol);

    while (1) {
        int key = readKey();
        if (key == KEY_NONE) continue; // No redraw if no input

        int moved = 0;
        
        if (key == KEY_LEFT) {
            cursorCol--;
            if (cursorCol < 0) cursorCol = cols - 1; // Wrap or clamp? Let's wrap.
            moved = 1;
        } else if (key == KEY_RIGHT) {
            cursorCol++;
            if (cursorCol >= cols) cursorCol = 0;
            moved = 1;
        } else if (key == KEY_ENTER) {
            if (colonnePleine(grille, cursorCol, rows)) {
                // Flash error?
                printf("\a"); // Bell
            } else {
                if (insererPion(grille, cursorCol, symbole, rows, cols)) {
                    if (playedCol) *playedCol = cursorCol;
                    return 1;
                }
            }
        } else if (key == KEY_ESC) {
            return -99;
        }
        
        if (moved) {
            clearScreen();
            printf("\033[1;36m=== Tour Joueur %d (%c) ===\033[0m\n", joueur, symbole);
            if (params->modeJeu == 1 || joueur == 1) {
                printf("Utilisez GAUCHE/DROITE pour viser, ENTREE pour jouer, ESC pour sauver/quitter.\n");
            }
            afficherGrilleTUI(grille, rows, cols, params, cursorCol);
        }
    }
}

// fonction pour v\xe9rifier la victoire d'un joueur
int verifierVictoire(char **grille, int rows, int cols, char symbole) {
    return verifierGrille(grille, rows, cols, symbole);
}

// fonction pour v\xe9rifier si la grille est pleine
static int grillePleine(char **grille, int rows, int cols) {
    for (int j = 0; j < cols; j++) {
        if (!colonnePleine(grille, j, rows))
            return 0;
    }
    return 1;
}

// fonction pour jouer une partie compl\xe8te
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
        clearScreen();
        printf("Partie reprise. Joueur %d a vous !\n", joueur);
        sleep(1);
    }

    time_t startTime = time(NULL);
    char symbole;

    // Replay recording
    int moves[MAX_MOVES_TOTAL]; // Fixed size macro
    int moveCount = 0;

    // Resume moves
    if (saveToResume != NULL) {
        if (saveToResume->moveCount <= MAX_MOVES_TOTAL) {
             memcpy(moves, saveToResume->moves, saveToResume->moveCount * sizeof(int));
             moveCount = saveToResume->moveCount;
        }
    }

    // Enable TUI Raw Mode just in case it was disabled
    enableRawMode();

    while (1) {
        // Autosave check
        if (params->autosave) {
             sauvegarderPartieSilencieuse(grille, rows, cols, joueur, params, moves, moveCount);
        }
        
        int result;
        int playedCol = -1;
        
        result = jouerTour(joueur, grille, rows, cols, params, &playedCol);
        
        if (result == -99) {
            disableRawMode(); // Need to type filename
            sauvegarderPartie(grille, rows, cols, joueur, params, moves, moveCount);
            enableRawMode();
            libererGrille(grille, rows);
            return; // Exit game
        }
        
        if (moveCount < MAX_MOVES_TOTAL) {
            moves[moveCount++] = playedCol;
        }

        symbole = (joueur == 1) ? params->symboleJ1 : params->symboleJ2;

        // v\xe9rifier victoire
        if (verifierVictoire(grille, rows, cols, symbole)) {
            clearScreen();
            afficherGrilleTUI(grille, rows, cols, params, -1);
            printf("\n\033[1;32m*** VICTOIRE ! Joueur %d (%c) a gagne ! ***\033[0m\n", joueur, symbole);
            printf("Appuyez sur une touche pour quitter...");
            readKey(); // Wait for key
            sauvegarderReplay(moves, moveCount, rows, cols, params, joueur);
            mettreAJourStats(joueur, (int)difftime(time(NULL), startTime));
            break;
        }

        // v\xe9rifier match nul
        if (grillePleine(grille, rows, cols)) {
            clearScreen();
            afficherGrilleTUI(grille, rows, cols, params, -1);
            printf("\n\033[1;33m*** MATCH NUL ! La grille est pleine. ***\033[0m\n");
             printf("Appuyez sur une touche pour quitter...");
            readKey(); // Wait for key
            sauvegarderReplay(moves, moveCount, rows, cols, params, 0);
            mettreAJourStats(0, (int)difftime(time(NULL), startTime));
            break;
        }

        // changer de joueur
        joueur = (joueur == 1) ? 2 : 1;
    }

    libererGrille(grille, rows);
    // disableRawMode handled by main or next menu loop
}
