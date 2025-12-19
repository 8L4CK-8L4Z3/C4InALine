#include <stdio.h>
#include <string.h>
#include <stdlib.h> // for system
#include "commun.h"
#include "ui.h"
#include "input.h"

void choisirCouleurTUI(int *colorRef, int playerNum) {
    int selected = *colorRef;
    if (selected < 1 || selected > 6) selected = 1;
    
    while(1) {
        clearScreen();
        printf("\n--- Couleur Joueur %d ---\n", playerNum);
        printf("Utilisez HAUT/BAS pour choisir, ENTREE pour valider.\n\n");
        
        for(int i=1; i<=6; i++) {
            const char *name = "";
            const char *ansi = "";
            switch(i) {
                case 1: name="Rouge"; ansi="\033[31m"; break;
                case 2: name="Vert"; ansi="\033[32m"; break;
                case 3: name="Jaune"; ansi="\033[33m"; break;
                case 4: name="Bleu"; ansi="\033[34m"; break;
                case 5: name="Magenta"; ansi="\033[35m"; break;
                case 6: name="Cyan"; ansi="\033[36m"; break;
            }
            
            if (i == selected) {
                printf("  > %s%s\033[0m\n", ansi, name);
            } else {
                printf("    %s\n", name);
            }
        }
        
        int key = readKey();
        if (key == KEY_UP) {
            selected--; if(selected < 1) selected = 6;
        } else if (key == KEY_DOWN) {
            selected++; if(selected > 6) selected = 1;
        } else if (key == KEY_ENTER) {
            *colorRef = selected;
            return;
        }
    }
}

const char* getColorName(int c) {
    switch(c) {
        case 1: return "Rouge";
        case 2: return "Vert";
        case 3: return "Jaune";
        case 4: return "Bleu";
        case 5: return "Magenta";
        case 6: return "Cyan";
        default: return "Blanc";
    }
}

// Gère les paramètres du jeu (TUI version)
void gererParametres(ParametresJeu *params) {
    int selected = 1;
    const int numOptions = 7;
    
    // Ensure raw mode is enabled for this function
    enableRawMode();
    
    while(1) {
        clearScreen();
        printf("\n=== Parametres de jeu ===\n");
        printf("Utilisez HAUT/BAS pour naviguer, ENTREE pour modifier.\n\n");

        const char *opts[] = {
            "1. Taille de la grille",
            "2. Symboles",
            "3. Couleur Joueur 1",
            "4. Couleur Joueur 2",
            "5. Temps limite",
            "6. Mode de jeu",
            "7. Retour"
        };
        
        for(int i=0; i<numOptions; i++) {
            if (i+1 == selected) printf("  \033[1;33m> ");
            else printf("    ");
            
            // Print value hints
            switch(i+1) {
                case 1: printf("%s [%d]", opts[i], params->tailleGrille); break;
                case 2: printf("%s [%c vs %c]", opts[i], params->symboleJ1, params->symboleJ2); break;
                case 3: printf("%s [%s]", opts[i], getColorName(params->colorJ1)); break;
                case 4: printf("%s [%s]", opts[i], getColorName(params->colorJ2)); break;
                case 5: printf("%s [%ds]", opts[i], params->tempsLimite); break;
                case 6: printf("%s [%s]", opts[i], params->modeJeu == 1 ? "PvP" : "PvE"); break;
                default: printf("%s", opts[i]);
            }
            
            if (i+1 == selected) printf("\033[0m");
            printf("\n");
        }
        
        int key = readKey();
        if (key == KEY_UP) {
            selected--; if(selected < 1) selected = numOptions;
        } else if (key == KEY_DOWN) {
            selected++; if(selected > numOptions) selected = 1;
        } else if (key == KEY_ENTER) {
            if (selected == 7) return;
            
            // For editing, we might need to briefly disable raw mode for scanf or implement a TUI editor.
            // Let's use simple scanf for values like integers/chars for now to save complexity, 
            // but wrapped nicely.
            disableRawMode();
            
            switch(selected) {
                case 1:
                    printf("\nEntrez taille (4-20): ");
                    scanf("%d", &params->tailleGrille);
                    if(params->tailleGrille > 20) params->tailleGrille = 20;
                    if(params->tailleGrille < 4) params->tailleGrille = 4;
                    getchar(); // Flush
                    break;
                case 2:
                    printf("\nSymbole J1: "); scanf("%c", &params->symboleJ1); getchar();
                    printf("Symbole J2: "); scanf("%c", &params->symboleJ2); getchar();
                    break;
                case 3:
                    enableRawMode();
                    choisirCouleurTUI(&params->colorJ1, 1);
                    // disableRawMode(); // Removed: loop needs raw mode
                    break;
                case 4:
                    enableRawMode();
                    choisirCouleurTUI(&params->colorJ2, 2);
                    // disableRawMode(); // Removed: loop needs raw mode
                    break;
                case 5:
                    printf("\nTemps limite (s): ");
                    scanf("%d", &params->tempsLimite);
                    getchar();
                    break;
                case 6:
                     // TUI toggle
                     params->modeJeu = (params->modeJeu == 1) ? 2 : 1;
                     break;
            }
            enableRawMode();
        }
    }
}
