#include <stdio.h>
#include <string.h>
#include <stdlib.h> // for system
#include "commun.h"

void choisirCouleur(int *colorRef, int playerNum) {
    int choix;
    printf("\n--- Couleur Joueur %d ---\n", playerNum);
    printf("1. Rouge\n");
    printf("2. Vert\n");
    printf("3. Jaune\n");
    printf("4. Bleu\n");
    printf("5. Magenta\n");
    printf("6. Cyan\n");
    printf("0. Blanc (Defaut)\n");
    printf("Choix : ");
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
    do {
        system("clear");
        printf("\n=== Parametres de jeu ===\n");
        printf("1. Taille de la grille (actuelle : %d)\n", params->tailleGrille);
        printf("2. Forme des pions (actuelle : J1:%c J2:%c)\n", params->symboleJ1, params->symboleJ2);
        printf("3. Couleur Joueur 1 (actuelle : %s)\n", getColorName(params->colorJ1));
        printf("4. Couleur Joueur 2 (actuelle : %s)\n", getColorName(params->colorJ2));
        printf("5. Temps limite par tour (actuel : %d secondes)\n", params->tempsLimite);
        printf("6. Mode de jeu (actuel : %s)\n", params->modeJeu == 1 ? "Joueur vs Joueur" : "Joueur vs Ordi");
        printf("7. Retour au menu principal\n");
        printf("Choisissez une option a modifier : ");
        scanf("%d", &choix);
        getchar(); // consommer '\n'

        switch(choix) {
            case 1:
                printf("Entrez taille de la grille (ex: 7, max 20): ");
                scanf("%d", &params->tailleGrille);
                if(params->tailleGrille > 20) params->tailleGrille = 20;
                if(params->tailleGrille < 4) params->tailleGrille = 4;
                getchar();
                break;
            case 2:
                printf("Entrez symbole J1 (char): ");
                scanf("%c", &params->symboleJ1); getchar();
                printf("Entrez symbole J2 (char): ");
                scanf("%c", &params->symboleJ2); getchar();
                break;
            case 3:
                choisirCouleur(&params->colorJ1, 1);
                break;
            case 4:
                choisirCouleur(&params->colorJ2, 2);
                break;
            case 5:
                printf("Entrez temps limite par tour (secondes): ");
                scanf("%d", &params->tempsLimite);
                getchar();
                break;
            case 6:
                printf("1. Joueur vs Joueur\n2. Joueur vs Ordi\nChoix: ");
                int m;
                scanf("%d", &m); getchar();
                if(m == 1 || m == 2) params->modeJeu = m;
                break;
            case 7:
                printf("Retour au menu principal...\n");
                break;
            default:
                printf("Option invalide.\n");
        }
    } while (choix != 7);
}
