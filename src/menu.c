#include <stdio.h>
#include "commun.h"
#include "ui.h"
#include "input.h" // For readKey, configureTerminal...
#include <sys/select.h>
#include <unistd.h>

#include <stdlib.h> // for system

// Helper generic menu
int menuSelection(const char *titre, const char *options[], int nbOptions) {
    int selection = 0;
    configureTerminal();
    
    while (1) {
        clearScreen();
        if (titre) {
             // If titre is provided, print it. If NULL, use Logo (main menu style)
             // Actually, main menu passed NULL.
             if (titre[0] == '\0') printLogo(); 
             else printCentered(titre);
        } else {
             printLogo();
        }
        
        printf("\n");
        for (int i = 0; i < nbOptions; i++) {
            char buf[200];
            if (i == selection) {
                // Highlight
                snprintf(buf, sizeof(buf), "-> %s <-", options[i]);
                printCentered(buf);
            } else {
                snprintf(buf, sizeof(buf), "   %s   ", options[i]);
                printCentered(buf);
            }
        }
        printCenteredPrompt("\nUtilisez les fleches haut/bas et Entree pour valider.");

        // Wait for input to avoid busy loop
        fd_set set;
        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);
        select(STDIN_FILENO + 1, &set, NULL, NULL, NULL);

        int key = readKey();
        if (key == KEY_UP) {
            selection = (selection - 1 + nbOptions) % nbOptions;
        } else if (key == KEY_DOWN) {
            selection = (selection + 1) % nbOptions;
        } else if (key == KEY_ENTER) {
            restoreTerminal();
            return selection + 1; // Return 1-based index
        }
    }
    restoreTerminal();
    return 0; // Should not reach
}

// Affiche le menu principal interactif
int afficherMenuInteractif() {
    const char *options[] = {
        "Jouer",
        "Parametres",
        "Rejouer",
        "Statistiques",
        "Quitter"
    };
    // Pass NULL as title to indicate "Use Logo" logic if I implemented it, 
    // but in menuSelection I said "If titre... else printLogo".
    // So passing NULL means printLogo.
    return menuSelection(NULL, options, 5);
}

// Affiche le menu principal (Deprecated/Legacy wrapper or removed)
void afficherMenu() {
    // Legacy support if needed, but we are replacing it.
    // We'll leave it empty or simple print if logic changes.
    // For now, let's keep it sync just in case, or remove.
    // The plan says "Replace ... with menuSelection".
}

// Affiche les replays (exemple)
void afficherReplays() {
    printCentered("\n=== Rejouer ===");
    printCentered("1. Partie vs IA - gagnant Joueur 1");
    printCentered("2. Partie 2 Joueurs - 30/03/2024");
    printCentered("Retour au menu principal...");
}
