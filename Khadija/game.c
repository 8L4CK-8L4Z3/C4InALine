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
void jouerPartie() {
    int rows, cols;
    printf("Entrez le nombre de lignes : ");
    scanf("%d", &rows);
    printf("Entrez le nombre de colonnes : ");
    scanf("%d", &cols);

    char **grille = creerGrille(rows, cols);

    int joueur = 1;
    char symbole;

    while (1) {
        afficherGrille(grille, rows, cols);

        // joueur joue son tour
        while (!jouerTour(joueur, grille, rows, cols)) {

        }

        symbole = (joueur == 1) ? 'X' : 'O';

        // vérifier victoire
        if (verifierVictoire(grille, rows, cols, symbole)) {
            afficherGrille(grille, rows, cols);
            printf("\n Joueur %d (%c) a gagné !\n", joueur, symbole);
            break;
        }

        // vérifier match nul
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
