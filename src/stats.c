#include <stdio.h>
#include "stats.h"

#define STATS_FILE "stats.dat"

void mettreAJourStats(int winner, int tempsDuree) {
    StatsJeu stats = {0,0,0,0};
    FILE *f = fopen(STATS_FILE, "rb");
    if (f) {
        fread(&stats, sizeof(StatsJeu), 1, f);
        fclose(f);
    }

    stats.partiesJouees++;
    stats.tempsTotalJoue += tempsDuree;
    if (winner == 1) stats.victoiresJ1++;
    else if (winner == 2) stats.victoiresJ2++;

    f = fopen(STATS_FILE, "wb");
    if (f) {
        fwrite(&stats, sizeof(StatsJeu), 1, f);
        fclose(f);
    }
}

void afficherStats() {
    StatsJeu stats = {0,0,0,0};
    FILE *f = fopen(STATS_FILE, "rb");
    if (f) {
        fread(&stats, sizeof(StatsJeu), 1, f);
        fclose(f);
    } else {
        printf("Aucune statistique disponible.\n");
        return;
    }

    printf("\n=== Statistiques Globales ===\n");
    printf("Parties Jouees : %d\n", stats.partiesJouees);
    printf("Victoires J1 : %d\n", stats.victoiresJ1);
    printf("Victoires J2/Ordi : %d\n", stats.victoiresJ2);
    printf("Temps total joue : %ds\n", stats.tempsTotalJoue);
}
