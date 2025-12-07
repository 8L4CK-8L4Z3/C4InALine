#ifndef GAME_H
#define GAME_H
#include<stdlib.h>
#include<stdio.h>

int jouerTour(int joueur, char **grille, int rows, int cols);
int verifierVictoire(char **grille, int rows, int cols, char symbole);
void jouerPartie();
#endif //GAME_H
