#include <stdio.h>
#include <stdlib.h>
#include "commun.h"
#include "ui.h"
#include "input.h"

// Returns choice 1-6
int afficherMenuTUI() {
    int selected = 1;
    const int numOptions = 6;
    const char *options[] = {
        "1. Jouer",
        "2. Parametres",
        "3. Charger",
        "4. Rejouer",
        "5. Statistiques",
        "6. Quitter"
    };

    while (1) {
        clearScreen();
        printLogo();
        printf("\n\n");
        
        // Draw Menu
        for (int i = 0; i < numOptions; i++) {
            if (i + 1 == selected) {
                printf("  \033[1;33m> %s\033[0m\n", options[i]); // Highlight
            } else {
                printf("    %s\n", options[i]);
            }
        }
        
        printf("\n(Utilisez fleches HAUT/BAS et ENTREE)\n");

        int key = readKey();
        if (key == KEY_UP) {
            selected--;
            if (selected < 1) selected = numOptions;
        } else if (key == KEY_DOWN) {
            selected++;
            if (selected > numOptions) selected = 1;
        } else if (key == KEY_ENTER) {
            return selected;
        }
    }
}

// Legacy function kept if needed elsewhere, but mainly replaced
void afficherMenu() {
    // Empty or wrapper
}
