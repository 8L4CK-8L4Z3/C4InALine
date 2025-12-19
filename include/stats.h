#ifndef STATS_H
#define STATS_H

typedef struct {
    int partiesJouees;
    int victoiresJ1;
    int victoiresJ2; // or AI
    int tempsTotalJoue; // in seconds
} StatsJeu;

void mettreAJourStats(int winner, int tempsDuree);
void afficherStats();

#endif
