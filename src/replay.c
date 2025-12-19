#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "replay.h"
#include "grid.h"

#define REPLAY_FILE "replays.dat"
#define MAX_REPLAYS 10

ReplayData replays[MAX_REPLAYS];
int nbReplays = 0;

void chargerReplaysDepuisFichier() {
    FILE *f = fopen(REPLAY_FILE, "rb");
    if (f) {
        nbReplays = fread(replays, sizeof(ReplayData), MAX_REPLAYS, f);
        fclose(f);
    } else {
        nbReplays = 0;
    }
}

void ecrireReplaysFichier() {
    FILE *f = fopen(REPLAY_FILE, "wb");
    if (f) {
        fwrite(replays, sizeof(ReplayData), nbReplays, f);
        fclose(f);
    }
}

void sauvegarderReplay(int *moves, int moveCount, int rows, int cols, ParametresJeu *params, int winner) {
    chargerReplaysDepuisFichier();

    if (nbReplays >= MAX_REPLAYS) {
         // Shift
        for(int i=0; i<nbReplays-1; i++) {
            replays[i] = replays[i+1];
        }
        nbReplays--;
    }

    ReplayData rd;
    snprintf(rd.nomReplay, 50, "Replay %d (%dx%d) - Vainqueur: %s", nbReplays+1, rows, cols, winner == 0 ? "Nul" : (winner == 1 ? "J1" : "J2"));
    
    // Copy moves
    memcpy(rd.moves, moves, moveCount * sizeof(int));
    rd.moveCount = moveCount;
    rd.rows = rows;
    rd.cols = cols;
    rd.params = *params;
    rd.winner = winner;

    replays[nbReplays++] = rd;
    ecrireReplaysFichier();
    printf("Replay sauvegarde.\n");
}

void afficherListeReplays() {
    chargerReplaysDepuisFichier();
    if (nbReplays == 0) {
        printf("Aucun replay disponible.\n");
        return;
    }
    printf("\n=== Replays ===\n");
    for(int i=0; i<nbReplays; i++) {
        printf("%d. %s\n", i+1, replays[i].nomReplay);
    }
}

void visionnerReplay() {
    chargerReplaysDepuisFichier();
    if (nbReplays == 0) return;

    afficherListeReplays();
    int choix;
    printf("Choisir un replay : ");
    if (scanf("%d", &choix) != 1) return;
    int c; while ((c = getchar()) != '\n' && c != EOF); // Flush

    if (choix < 1 || choix > nbReplays) {
        printf("Invalide.\n");
        return;
    }

    ReplayData *rd = &replays[choix-1];
    char **grille = creerGrille(rd->rows, rd->cols);
    
    printf("Demarrage du replay...\n");
    afficherGrille(grille, rd->rows, rd->cols, &rd->params);
    sleep(1);

    int joueur = 1;
    for(int i=0; i<rd->moveCount; i++) {
        printf("Tour %d : Joueur %d joue colonne %d\n", i+1, joueur, rd->moves[i]);
        char sym = (joueur == 1) ? rd->params.symboleJ1 : rd->params.symboleJ2;
        insererPion(grille, rd->moves[i], sym, rd->rows, rd->cols);
        afficherGrille(grille, rd->rows, rd->cols, &rd->params);
        joueur = (joueur == 1) ? 2 : 1;
        sleep(1);
    }

    printf("Fin du replay.\n");
    libererGrille(grille, rd->rows);
}
