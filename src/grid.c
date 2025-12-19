#include "grid.h"

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

void afficherGrille(char **grille, int rows, int cols){
    printf("\n  ");
    for(int c=0;c<cols;c++){
        printf("%d ",c);//afficher le num de chaque col
    }
    printf("\n");
    for(int i=0;i<rows;i++){
        printf("%d ",i);
        for(int j=0;j<cols;j++){
            printf( "%c ",grille[i][j]);
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
