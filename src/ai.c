#include "ai.h"
#include "grid.h"
#include <stdio.h>

// Helper: Check if a move wins
int simulateWin(char **grille, int col, int rows, int cols, char symbol) {
    if (!insererPion(grille, col, symbol, rows, cols)) return 0;
    int win = verifierGrille(grille, rows, cols, symbol);
    
    // Undo move (remove top symbol in col)
    for (int i = 0; i < rows; i++) {
        if (grille[i][col] != '.') {
            grille[i][col] = '.';
            break;
        }
    }
    return win;
}

int calculerCoupOrdi(char **grille, int rows, int cols, char symboleOrdi, char symboleJoueur) {
    // 1. Check if AI can win
    for (int c = 0; c < cols; c++) {
        // We need a copy or simulate
        // Since we don't have a copy function handy, we use insererPion and undo it.
        // But insererPion modifies the grid.
        // We need to be careful. simulateWin does inserer and undo.
        if (simulateWin(grille, c, rows, cols, symboleOrdi)) {
            return c;
        }
    }

    // 2. Block opponent win
    for (int c = 0; c < cols; c++) {
        if (simulateWin(grille, c, rows, cols, symboleJoueur)) {
            return c;
        }
    }

    // 3. Random valid move
    int validMoves[cols];
    int count = 0;
    for (int c = 0; c < cols; c++) {
        if (!colonnePleine(grille, c, rows)) {
            validMoves[count++] = c;
        }
    }

    if (count > 0) {
        return validMoves[rand() % count];
    }

    return 0; // Should not happen if grid not full
}
