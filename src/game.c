#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "grid.h"
// fonction pour jouer un tour
int jouerTour(int joueur, char **grille, int rows, int cols) {
    char symbole = (joueur == 1) ? 'X' : 'O';
    int col;

    printf("Joueur %d (%c), choisissez une colonne : ", joueur, symbole);
    scanf("%d", &col);

    if (!insererPion(grille, col, symbole, rows, cols)) {
        printf(" Colonne invalide ou pleine ! Choisissez une autre.\n");
        return 0;
    }

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
void jouerPartie(int rows, int cols) {
    char **grille = creerGrille(rows, cols);

    int joueur = 1;
    char symbole;

    while (1) {
        afficherGrille(grille, rows, cols);

        // joueur joue son tour
        while (!jouerTour(joueur, grille, rows, cols)) {

        }

        symbole = (joueur == 1) ? 'X' : 'O';

        // v\xe9rifier victoire
        if (verifierVictoire(grille, rows, cols, symbole)) {
            afficherGrille(grille, rows, cols);
            printf("\n Joueur %d (%c) a gagn\xe9 !\n", joueur, symbole);
            break;
        }

        // v\xe9rifier match nul
        if (grillePleine(grille, rows, cols)) {
            afficherGrille(grille, rows, cols);
            printf("\n Match nul ! La grille est pleine.\n");
            break;
        }

        // changer de joueur
        joueur = (joueur == 1) ? 2 : 1;
    }

    libererGrille(grille, rows);
}
