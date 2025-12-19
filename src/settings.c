#include <stdio.h>
#include <string.h>
#include "commun.h"

// G\xe8re les param\xe8tres du jeu
void gererParametres(ParametresJeu *params) {
    int choix;
    do {
        printf("\n=== Parametres de jeu ===\n");
        printf("1. Taille de la grille (actuelle : %d)\n", params->tailleGrille);
        printf("2. Forme des pions (actuelle : J1:%c J2:%c)\n", params->symboleJ1, params->symboleJ2);
        printf("3. Couleur des pions (actuelle : %s)\n", params->couleurPions);
        printf("4. Temps limite par tour (actuel : %d secondes)\n", params->tempsLimite);
        printf("5. Mode de jeu (actuel : %s)\n", params->modeJeu == 1 ? "Joueur vs Joueur" : "Joueur vs Ordi");
        printf("6. Retour au menu principal\n");
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
                printf("Entrez couleur des pions (ex: rouge, jaune): ");
                fgets(params->couleurPions, sizeof(params->couleurPions), stdin);
                params->couleurPions[strcspn(params->couleurPions, "\n")] = 0;
                break;
            case 4:
                printf("Entrez temps limite par tour (secondes): ");
                scanf("%d", &params->tempsLimite);
                getchar();
                break;
            case 5:
                printf("1. Joueur vs Joueur\n2. Joueur vs Ordi\nChoix: ");
                int m;
                scanf("%d", &m); getchar();
                if(m == 1 || m == 2) params->modeJeu = m;
                break;
            case 6:
                printf("Retour au menu principal...\n");
                break;
            default:
                printf("Option invalide.\n");
        }
    } while (choix != 6);
}
