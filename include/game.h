#ifndef GAME_H
#define GAME_H
#include<stdlib.h>
#include<stdio.h>
#include "commun.h"

int jouerTour(int joueur, char **grille, int rows, int cols, ParametresJeu *params, int *playedCol);
int verifierVictoire(char **grille, int rows, int cols, char symbole);
void jouerPartie(ParametresJeu *params, PartieSauvegardee *saveToResume); // Added saveToResume support
#endif //GAME_H
