#ifndef REPLAY_H
#define REPLAY_H

#include "commun.h"

// Use MAX_MOVES_TOTAL from commun.h
#define MAX_MOVES MAX_MOVES_TOTAL

typedef struct {
    char nomReplay[50];
    int moves[MAX_MOVES];
    int moveCount;
    int rows;
    int cols;
    ParametresJeu params;
    int winner; // 0=draw, 1=J1, 2=J2
} ReplayData;

void sauvegarderReplay(int *moves, int moveCount, int rows, int cols, ParametresJeu *params, int winner);
void afficherListeReplays();
void visionnerReplay();

#endif
