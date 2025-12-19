#include <stdio.h>
#include <string.h>
#include <stdlib.h> // for system
#include "commun.h"
#include "ui.h"

void choisirCouleur(int *colorRef, int playerNum) {
    int choix;
    char buf[100];
    snprintf(buf, sizeof(buf), "\n--- Couleur Joueur %d ---", playerNum);
    printCentered(buf);
    printCentered("1. Rouge");
    printCentered("2. Vert");
    printCentered("3. Jaune");
    printCentered("4. Bleu");
    printCentered("5. Magenta");
    printCentered("6. Cyan");
    printCentered("0. Blanc (Defaut)");
    printCenteredPrompt("Choix : ");
    
    scanf("%d", &choix);
    getchar();
    if (choix >= 0 && choix <= 6) {
        *colorRef = choix;
    } else {
        printf("Choix invalide, conservation de la couleur.\n");
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

// Gère les paramètres du jeu
void gererParametres(ParametresJeu *params) {
    int choix;
    char buf[200];
    do {
        clearScreen();
        printCentered("\n=== Parametres de jeu ===");
        snprintf(buf, sizeof(buf), "1. Taille de la grille (actuelle : %d)", params->tailleGrille); printCentered(buf);
        snprintf(buf, sizeof(buf), "2. Forme des pions (actuelle : J1:%c J2:%c)", params->symboleJ1, params->symboleJ2); printCentered(buf);
        snprintf(buf, sizeof(buf), "3. Couleur Joueur 1 (actuelle : %s)", getColorName(params->colorJ1)); printCentered(buf);
        snprintf(buf, sizeof(buf), "4. Couleur Joueur 2 (actuelle : %s)", getColorName(params->colorJ2)); printCentered(buf);
        snprintf(buf, sizeof(buf), "5. Temps limite par tour (actuel : %d secondes)", params->tempsLimite); printCentered(buf);
        snprintf(buf, sizeof(buf), "6. Mode de jeu (actuel : %s)", params->modeJeu == 1 ? "Joueur vs Joueur" : "Joueur vs Ordi"); printCentered(buf);
        char diff[20];
        if(params->difficulty == 1) strcpy(diff, "Facile");
        else if(params->difficulty == 3) strcpy(diff, "Difficile");
        else strcpy(diff, "Moyen");
        snprintf(buf, sizeof(buf), "7. Difficulte IA (actuelle : %s)", diff); printCentered(buf);
        printCentered("8. Retour au menu principal");
        printCenteredPrompt("Choisissez une option a modifier : ");
        
        scanf("%d", &choix);
        getchar(); // consommer '\n'

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
            case 6:
                printCentered("1. Joueur vs Joueur");
                printCentered("2. Joueur vs Ordi");
                printCenteredPrompt("Choix: ");
                int m;
                scanf("%d", &m); getchar();
                if(m == 1 || m == 2) params->modeJeu = m;
                break;
            case 7:
                printCentered("1. Facile");
                printCentered("2. Moyen");
                printCentered("3. Difficile");
                printCenteredPrompt("Choix: ");
                int d;
                scanf("%d", &d); getchar();
                if(d >= 1 && d <= 3) params->difficulty = d;
                break;
            case 8:
                printf("Retour au menu principal...\n");
                break;
            default:
                printf("Option invalide.\n");
        }
    } while (choix != 8);
}
