#include <stdio.h>
#include <string.h>
#include "commun.h"

// Implements silent autosave
void sauvegarderPartieSilencieuse(char **grille, int rows, int cols, int joueurCourant, ParametresJeu *params, int *moves, int moveCount) {
    PartieSauvegardee autoSave;
    strcpy(autoSave.nomPartie, "AUTOSAVE");
    autoSave.parametres = *params;
    autoSave.rows = rows;
    autoSave.cols = cols;
    autoSave.joueurCourant = joueurCourant;
    
    // Copy moves
    if (moveCount <= MAX_MOVES_TOTAL) {
        memcpy(autoSave.moves, moves, moveCount * sizeof(int));
        autoSave.moveCount = moveCount;
    } else {
        autoSave.moveCount = 0;
    }

    // Grid copy
    if (rows > MAX_ROWS || cols > MAX_COLS) return;
    memset(autoSave.grille, 0, sizeof(autoSave.grille));
    for(int i=0; i<rows; i++) {
        for(int j=0; j<cols; j++) {
            autoSave.grille[i][j] = grille[i][j];
        }
    }

    FILE *f = fopen("autosave.dat", "wb");
    if (f) {
        fwrite(&autoSave, sizeof(PartieSauvegardee), 1, f);
        fclose(f);
    }
}
