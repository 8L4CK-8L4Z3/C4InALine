#include <stdio.h>
#include "commun.h"
#include "ui.h"

#include <stdlib.h> // for system

// Affiche le menu principal
void afficherMenu() {
    clearScreen();
    printLogo();
    printCentered("1. Jouer");
    printCentered("2. Parametres");
    printCentered("3. Rejouer");
    printCentered("4. Statistiques");
    printCentered("5. Quitter");
    printCenteredPrompt("Veuillez choisir une option : ");
}

// Affiche les replays (exemple)
void afficherReplays() {
    printCentered("\n=== Rejouer ===");
    printCentered("1. Partie vs IA - gagnant Joueur 1");
    printCentered("2. Partie 2 Joueurs - 30/03/2024");
    printCentered("Retour au menu principal...");
}
