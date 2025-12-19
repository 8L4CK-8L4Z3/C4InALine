#include "ai.h"
#include "grid.h"
#include <stdio.h>
#include <limits.h>

// Helper: Check if a move wins
static int simulateWin(char **grille, int col, int rows, int cols, char symbol) {
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

// Minimax with Alpha-Beta
// Returns score. Best move is not returned directly, used recursively.
static int minimax(char **grille, int depth, int alpha, int beta, int maximizingPlayer, int rows, int cols, char symboleOrdi, char symboleJoueur) {
    // Check terminal states
    if (verifierGrille(grille, rows, cols, symboleOrdi)) return 1000 + depth;
    if (verifierGrille(grille, rows, cols, symboleJoueur)) return -1000 - depth;
    
    // Check draw
    int full = 1;
    for(int j=0; j<cols; j++) if(!colonnePleine(grille, j, rows)) full=0;
    if (full || depth == 0) return 0; // Heuristic could be added here

    if (maximizingPlayer) {
        int maxEval = INT_MIN;
        for (int c = 0; c < cols; c++) {
            if (!colonnePleine(grille, c, rows)) {
                insererPion(grille, c, symboleOrdi, rows, cols);
                int eval = minimax(grille, depth - 1, alpha, beta, 0, rows, cols, symboleOrdi, symboleJoueur);
                // Undo
                for (int i = 0; i < rows; i++) { if (grille[i][c] != '.') { grille[i][c] = '.'; break; } }
                
                if (eval > maxEval) maxEval = eval;
                if (eval > alpha) alpha = eval;
                if (beta <= alpha) break;
            }
        }
        return maxEval;
    } else {
        int minEval = INT_MAX;
        for (int c = 0; c < cols; c++) {
            if (!colonnePleine(grille, c, rows)) {
                insererPion(grille, c, symboleJoueur, rows, cols);
                int eval = minimax(grille, depth - 1, alpha, beta, 1, rows, cols, symboleOrdi, symboleJoueur);
                // Undo
                for (int i = 0; i < rows; i++) { if (grille[i][c] != '.') { grille[i][c] = '.'; break; } }
                
                if (eval < minEval) minEval = eval;
                if (eval < beta) beta = eval;
                if (beta <= alpha) break;
            }
        }
        return minEval;
    }
}

int calculerCoupOrdi(char **grille, int rows, int cols, char symboleOrdi, char symboleJoueur, int difficulty) {
    int validMoves[cols];
    int count = 0;
    for (int c = 0; c < cols; c++) {
        if (!colonnePleine(grille, c, rows)) {
            validMoves[count++] = c;
        }
    }
    if (count == 0) return 0;

    // Difficulty 1: Easy (Random)
    if (difficulty == 1) {
        return validMoves[rand() % count];
    }

    // Difficulty 2: Medium (Block/Win, else Random)
    if (difficulty == 2) {
        // 1. Check win
        for (int c = 0; c < cols; c++) {
            if (simulateWin(grille, c, rows, cols, symboleOrdi)) return c;
        }
        // 2. Block
        for (int c = 0; c < cols; c++) {
            if (simulateWin(grille, c, rows, cols, symboleJoueur)) return c;
        }
        // 3. Random
        return validMoves[rand() % count];
    }

    // Difficulty 3: Hard (Minimax)
    if (difficulty == 3) {
        int bestMove = validMoves[0];
        int maxEval = INT_MIN;
        int depth = 5; // Search depth

        for (int c = 0; c < cols; c++) {
            if (!colonnePleine(grille, c, rows)) {
                insererPion(grille, c, symboleOrdi, rows, cols);
                int eval = minimax(grille, depth - 1, INT_MIN, INT_MAX, 0, rows, cols, symboleOrdi, symboleJoueur);
                // Undo
                for (int i = 0; i < rows; i++) { if (grille[i][c] != '.') { grille[i][c] = '.'; break; } }

                if (eval > maxEval) {
                    maxEval = eval;
                    bestMove = c;
                }
            }
        }
        return bestMove;
    }
    
    return validMoves[rand() % count]; // Fallback
}
