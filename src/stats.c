#include <stdio.h>
#include "stats.h"
#include "ui.h"

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
        printCentered("Aucune statistique disponible.");
        printCentered("Appuyez sur Entree pour revenir au menu...");
        getchar();
        return;
    }

    printCentered("\n=== Statistiques Globales ===");
    char buf[100];
    snprintf(buf, sizeof(buf), "Parties Jouees : %d", stats.partiesJouees); printCentered(buf);
    snprintf(buf, sizeof(buf), "Victoires J1 : %d", stats.victoiresJ1); printCentered(buf);
    snprintf(buf, sizeof(buf), "Victoires J2/Ordi : %d", stats.victoiresJ2); printCentered(buf);
    snprintf(buf, sizeof(buf), "Temps total joue : %ds", stats.tempsTotalJoue); printCentered(buf);
    
    printCentered("Appuyez sur Entree pour revenir au menu...");
    getchar();
}
