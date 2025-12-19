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
        printf("L'ordinateur reflechit...\n");
        // sleep(1);
        col = calculerCoupOrdi(grille, rows, cols, params->symboleJ2, params->symboleJ1);
        printf("L'ordinateur joue en colonne %d\n", col);
    } else {
        printf("Joueur %d (%c), choisissez une colonne (0-%d) ou -99 pour sauver : ", joueur, symbole, cols-1);
        
        // Time limit logic using select
        fd_set set;
        struct timeval timeout;
        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);
        
        timeout.tv_sec = params->tempsLimite;
        timeout.tv_usec = 0;

        int rv = select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout);
        if (rv == -1) {
            perror("select"); 
            return 0;
        } else if (rv == 0) {
            printf("\nTemps ecoule ! Vous passez votre tour (coup aleatoire).\n");
            do {
                col = rand() % cols;
            } while (colonnePleine(grille, col, rows));
        } else {
            if (scanf("%d", &col) != 1) {
                printf("Entree invalide.\n");
                viderBuffer();
                return 0;
            }
        }
    }

    if (col == -99) return -99;

    if (!insererPion(grille, col, symbole, rows, cols)) {
        printf(" Colonne invalide ou pleine ! Choisissez une autre.\n");
        return 0;
    }

    if (playedCol) *playedCol = col;
    return 1;
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
        printf("Partie reprise. Joueur %d a vous !\n", joueur);
    }

    time_t startTime = time(NULL);
    char symbole;

    // Replay recording
    int moves[MAX_MOVES];
    int moveCount = 0;

    // Resume moves
    if (saveToResume != NULL) {
        if (saveToResume->moveCount <= MAX_MOVES) {
             memcpy(moves, saveToResume->moves, saveToResume->moveCount * sizeof(int));
             moveCount = saveToResume->moveCount;
        }
    }

    // Recover moves if resuming? No easy way unless saved. 
    // Assuming replay is for new games or just appended. 
    // If resuming, moveCount starts at 0 effectively losing history of resumed game.
    // For full compliance, save file should store moves too. 
    // I will skip move recovery on resume for now as it wasn't strictly requested to REPLAY resumed games fully from start.

    while (1) {
        afficherGrille(grille, rows, cols, params);
        
        // Autosave check
        if (params->autosave) {
             sauvegarderPartieSilencieuse(grille, rows, cols, joueur, params, moves, moveCount);
        }
        
        int result;
        // We need to capture the column played to store in moves[]
        // jouerTour currently returns 1 on success but doesn't return the column.
        // We need to modify jouerTour to return the column played or use a pointer.
        // Or simpler: check which column changed? No.
        // I will modify jouerTour signature to return the column via pointer.
        
        int playedCol = -1;
        do {
            // Updated signature call needed, but I need to update prototype first.
            // Wait, I can't update signature easily without breaking header.
            // Actually I can, I'm editing game.c
            // Let's rely on retrieving the move.
            // Since `jouerTour` does the insertion, I don't know which col.
            // I should modify `jouerTour`.
            
            // Let's modify `jouerTour` to accept `int *lastCol`.
            result = jouerTour(joueur, grille, rows, cols, params, &playedCol);
            
            if (result == -99) {
                sauvegarderPartie(grille, rows, cols, joueur, params, moves, moveCount);
                libererGrille(grille, rows);
                return; // Exit game
            }
        } while (result == 0);
        
        if (moveCount < MAX_MOVES) {
            moves[moveCount++] = playedCol;
        }

        symbole = (joueur == 1) ? params->symboleJ1 : params->symboleJ2;

        // v\xe9rifier victoire
        if (verifierVictoire(grille, rows, cols, symbole)) {
            afficherGrille(grille, rows, cols, params);
            printf("\n Joueur %d (%c) a gagn\xe9 !\n", joueur, symbole);
            sauvegarderReplay(moves, moveCount, rows, cols, params, joueur);
            mettreAJourStats(joueur, (int)difftime(time(NULL), startTime));
            break;
        }

        // v\xe9rifier match nul
        if (grillePleine(grille, rows, cols)) {
            afficherGrille(grille, rows, cols, params);
            printf("\n Match nul ! La grille est pleine.\n");
            sauvegarderReplay(moves, moveCount, rows, cols, params, 0);
            mettreAJourStats(0, (int)difftime(time(NULL), startTime));
            break;
        }

        // changer de joueur
        joueur = (joueur == 1) ? 2 : 1;
    }

    libererGrille(grille, rows);
}
