#include <stdio.h>
#include "commun.h"

// Affiche le menu principal
void afficherMenu() {
    printf("\n=== Menu Principal ===\n");
    printf("1. Jouer\n");
    printf("2. Parametres\n");
    printf("3. Sauvegarder \n");
    printf("4. Rejouer\n");
    printf("5. Quitter\n");
    printf("Veuillez choisir une option : ");
}

// Affiche les replays (exemple)
void afficherReplays() {
    printf("\n=== Rejouer ===\n");
    printf("1. Partie vs IA - gagnant Joueur 1\n");
    printf("2. Partie 2 Joueurs - 30/03/2024\n");
    printf("Retour au menu principal...\n");
}
