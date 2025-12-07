#ifndef GRID_H
#define GRID_H
#include<stdlib.h>
#include<stdio.h>

// créer une grille
char** creerGrille(int rows, int cols);

// libérer la grille
void libererGrille(char **grille, int rows);

// afficher la grille
void afficherGrille(char **grille, int rows, int cols);

// insérer un pion dans une colonne
int insererPion(char **grille, int col, char symbole, int rows, int cols);

// vérifier si une colonne est pleine
int colonnePleine(char **grille, int col, int rows);

//verifier victoire
int verifierGrille(char **grille, int rows, int cols, char symbole);


#endif //GRID_H
