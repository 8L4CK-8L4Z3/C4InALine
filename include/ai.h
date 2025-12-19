#ifndef AI_H
#define AI_H

#include <stdlib.h>
#include <time.h>

// Calcule le coup de l'ordinateur
// Retourne l'index de la colonne choisie
// difficulty: 1=Easy, 2=Medium, 3=Hard
int calculerCoupOrdi(char **grille, int rows, int cols, char symboleOrdi, char symboleJoueur, int difficulty);

#endif
