#include <stdio.h>
#include "commun.h"
#include "ui.h"

#include <stdlib.h> // for system

// Affiche le menu principal
void afficherMenu() {
    clearScreen();
    printCentered("\n\033[1;36m=== PUISSANCE 4 ===\033[0m");
    printCentered("1. Jouer");
    printCentered("2. Parametres");
    printCentered("3. Sauvegarder");
    printCentered("4. Rejouer");
    printCentered("5. Statistiques");
    printCentered("6. Quitter");
    printCentered("Veuillez choisir une option : ");
}

// Affiche les replays (exemple)
void afficherReplays() {
    printCentered("\n=== Rejouer ===");
    printCentered("1. Partie vs IA - gagnant Joueur 1");
    printCentered("2. Partie 2 Joueurs - 30/03/2024");
    printCentered("Retour au menu principal...");
}
