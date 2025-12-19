#include <stdio.h>
#include <string.h>
#include "grid.h"
#include "ui.h"

char** creerGrille(int rows, int cols){
    char **grille = malloc(rows * sizeof(char*));
    for(int i=0 ; i<rows ; i++){
        grille[i] = malloc(cols * sizeof(char));
        for(int j=0;j<cols ; j++){
            grille[i][j]='.';
        }
    }
    return grille;
}

void libererGrille(char **grille, int rows){
    for(int i=0;i<rows;i++){
        free(grille[i]);
    }
    free(grille);
}

// Helper for colors
const char* getAnsiColor(int c) {
    switch(c) {
        case 1: return "\033[31m"; // Red
        case 2: return "\033[32m"; // Green
        case 3: return "\033[33m"; // Yellow
        case 4: return "\033[34m"; // Blue
        case 5: return "\033[35m"; // Magenta
        case 6: return "\033[36m"; // Cyan
        default: return "\033[0m"; // White/Reset
    }
}

// Params: cursorCol is the column index where the player cursor is (-1 if none)
void afficherGrilleTUI(char **grille, int rows, int cols, ParametresJeu *params, int cursorCol) {
    // We assume clearScreen is called before
    
    // Print Cursor Row
    printf("\n  ");
    for(int c=0; c<cols; c++) {
        if (c == cursorCol) {
            printf("\033[1;37mV \033[0m"); // Cursor
        } else {
            printf("  ");
        }
    }
    printf("\n");

    // Print Grid
    for(int i=0; i<rows; i++){
        printf("  "); // Margin
        for(int j=0; j<cols; j++){
            char sym = grille[i][j];
            if (sym == '.') {
                printf("\033[90m. \033[0m"); // Dark grey for dots
            } else {
                const char *colorCode = "\033[0m";
                if (sym == params->symboleJ1) colorCode = getAnsiColor(params->colorJ1);
                else if (sym == params->symboleJ2) colorCode = getAnsiColor(params->colorJ2);
                
                printf("%s%c\033[0m ", colorCode, sym);
            }
        }
        printf("\n");
    }
    
    // Column numbers
    printf("  ");
    for(int c=0;c<cols;c++){
        printf("%d ", c%10);
    }
    printf("\n");
}

void afficherGrille(char **grille, int rows, int cols, ParametresJeu *params){
    afficherGrilleTUI(grille, rows, cols, params, -1);
}

int colonnePleine(char **grille, int col, int rows) {
    return grille[0][col] != '.';
}

int insererPion(char **grille, int col, char symbole, int rows, int cols){
    if (col < 0 || col >= cols)//colonne ne se trouve pas
            return 0;

        if (colonnePleine(grille, col, rows)) //colonne pleine
            return 0;

        // bas vers haut
        for (int i = rows - 1; i >= 0; i--) {
            if (grille[i][col] == '.') {
                grille[i][col] = symbole;
                return 1;
            }
        }
        return 0;
}

int verifierGrille(char **grille, int rows, int cols, char symbole) {

    // horiz
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols - 3; j++) {
            if (grille[i][j]     == symbole &&
                grille[i][j + 1] == symbole &&
                grille[i][j + 2] == symbole &&
                grille[i][j + 3] == symbole)
                return 1;
        }
    }

    //  verticale
    for (int j = 0; j < cols; j++) {
        for (int i = 0; i < rows - 3; i++) {
            if (grille[i][j]     == symbole &&
                grille[i + 1][j] == symbole &&
                grille[i + 2][j] == symbole &&
                grille[i + 3][j] == symbole)
                return 1;
        }
    }

    /* Diagonale \  */
    for (int i = 0; i < rows - 3; i++) {
        for (int j = 0; j < cols - 3; j++) {
            if (grille[i][j]     == symbole &&
                grille[i + 1][j + 1] == symbole &&
                grille[i + 2][j + 2] == symbole &&
                grille[i + 3][j + 3] == symbole)
                return 1;
        }
    }

    // Diagonale /
    for (int i = 0; i < rows - 3; i++) {
        for (int j = 3; j < cols; j++) {
            if (grille[i][j]     == symbole &&
                grille[i + 1][j - 1] == symbole &&
                grille[i + 2][j - 2] == symbole &&
                grille[i + 3][j - 3] == symbole)
                return 1;
        }
    }

    return 0; // pas de gagnant
}
