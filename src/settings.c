#include <stdio.h>
#include <string.h>
#include <stdlib.h> // for system
#include "commun.h"
#include "ui.h"

void choisirCouleur(int *colorRef, int playerNum) {
    char title[100];
    snprintf(title, sizeof(title), "Couleur Joueur %d", playerNum);
    const char *options[] = {
        "Rouge",
        "Vert",
        "Jaune",
        "Bleu",
        "Magenta",
        "Cyan",
        "Blanc (Defaut)"
    };
    int choix = menuSelection(title, options, 7);
    // menuSelection returns 1-based index: 1=Rouge, ..., 7=Blanc
    // Mapping: Rouge=1, Vert=2, Jaune=3, Bleu=4, Magenta=5, Cyan=6, Blanc=7 (or 0)
    // Existing code logic: 1..6 are colors, 0 is default (White).
    // Let's map 7 to 0.
    if (choix == 7) *colorRef = 0;
    else *colorRef = choix;
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

// Gère les paramètres du jeu
void gererParametres(ParametresJeu *params) {
    int choix;
    char opt1[100], opt2[100], opt3[100], opt4[100], opt5[100], opt6[100], opt7[100];
    
    do {
        char diff[20];
        if(params->difficulty == 1) strcpy(diff, "Facile");
        else if(params->difficulty == 3) strcpy(diff, "Difficile");
        else strcpy(diff, "Moyen");

        snprintf(opt1, sizeof(opt1), "Taille de la grille (%d)", params->tailleGrille);
        snprintf(opt2, sizeof(opt2), "Forme des pions (J1:%c J2:%c)", params->symboleJ1, params->symboleJ2);
        snprintf(opt3, sizeof(opt3), "Couleur Joueur 1 (%s)", getColorName(params->colorJ1));
        snprintf(opt4, sizeof(opt4), "Couleur Joueur 2 (%s)", getColorName(params->colorJ2));
        snprintf(opt5, sizeof(opt5), "Temps limite (%ds)", params->tempsLimite);
        snprintf(opt6, sizeof(opt6), "Mode de jeu (%s)", params->modeJeu == 1 ? "PvP" : "PvC");
        snprintf(opt7, sizeof(opt7), "Difficulte IA (%s)", diff);

        const char *options[] = {opt1, opt2, opt3, opt4, opt5, opt6, opt7, "Retour"};
        
        choix = menuSelection("Parametres de jeu", options, 8);

        switch(choix) {
            case 1:
                printCenteredPrompt("Entrez taille de la grille (ex: 7, max 20): ");
                scanf("%d", &params->tailleGrille);
                if(params->tailleGrille > 20) params->tailleGrille = 20;
                if(params->tailleGrille < 4) params->tailleGrille = 4;
                getchar();
                break;
            case 2:
                printCenteredPrompt("Entrez symbole J1 (char): ");
                scanf("%c", &params->symboleJ1); getchar();
                printCenteredPrompt("Entrez symbole J2 (char): ");
                scanf("%c", &params->symboleJ2); getchar();
                break;
            case 3:
                choisirCouleur(&params->colorJ1, 1);
                break;
            case 4:
                choisirCouleur(&params->colorJ2, 2);
                break;
            case 5:
                printCenteredPrompt("Entrez temps limite par tour (secondes): ");
                scanf("%d", &params->tempsLimite);
                getchar();
                break;
            case 6: {
                const char *modes[] = {"Joueur vs Joueur", "Joueur vs Ordi"};
                int m = menuSelection("Mode de Jeu", modes, 2);
                params->modeJeu = m;
                break;
            }
            case 7: {
                const char *diffs[] = {"Facile", "Moyen", "Difficile"};
                int d = menuSelection("Difficulte IA", diffs, 3);
                params->difficulty = d; // 1, 2, 3 maps directly
                break;
            }
            case 8:
                // Retour
                break;
            default:
                break;
        }
    } while (choix != 8);
}
