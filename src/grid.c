#include "grid.h"
#include "ui.h"
#include <string.h>

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

void afficherGrille(char **grille, int rows, int cols, ParametresJeu *params){
    int termWidth = getTerminalWidth();
    // Grid width calculation:
    // Header row: "  0 1 2 ..."
    // Each cell is 2 chars ("0 "), plus initial "  ".
    // Actually header: "  " + (cols * 2 chars) -> 2 + 2*cols.
    
    // Grid rows: "0 . . . "
    // "0 " (2 chars) + cols * 2 chars ("X ") -> 2 + 2*cols.
    
    int gridWidth = 2 + 2 * cols;
    int padding = (termWidth - gridWidth) / 2;
    if (padding < 0) padding = 0;
    
    // Print header
    printf("\n");
    for(int k=0; k<padding; k++) printf(" ");
    printf("  ");
    for(int c=0;c<cols;c++){
        printf("%d ",c);//afficher le num de chaque col
    }
    printf("\n");

    for(int i=0;i<rows;i++){
        for(int k=0; k<padding; k++) printf(" ");
        printf("%d ",i);
        for(int j=0;j<cols;j++){
            char sym = grille[i][j];
            if (sym == '.') {
                printf(". ");
            } else {
                const char *colorCode = "\033[0m";
                if (sym == params->symboleJ1) colorCode = getAnsiColor(params->colorJ1);
                else if (sym == params->symboleJ2) colorCode = getAnsiColor(params->colorJ2);
                
                printf("%s%c\033[0m ", colorCode, sym);
            }
        }
        printf("\n");
    }
    printf("\n");
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
