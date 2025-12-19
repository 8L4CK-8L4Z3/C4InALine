#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include "commun.h"

// Gère les paramètres du jeu
void gererParametres(ParametresJeu *params) {
    int choix;
    do {
        printf("\n=== Parametres de jeu ===\n");
        printf("1. Taille de la grille (actuelle : %d)\n", params->tailleGrille);
        printf("2. Forme des pions (actuelle : %s)\n", params->formePions);
        printf("3. Couleur des pions (actuelle : %s)\n", params->couleurPions);
        printf("4. Temps limite par tour (actuel : %d secondes)\n", params->tempsLimite);
        printf("5. Autosave (actuelle : %s)\n", params->autosave ? "Activee" : "Désactivee");
        printf("6. Retour au menu principal\n");
        printf("Choisissez une option a modifier : ");
        scanf("%d", &choix);
        getchar(); // consommer '\n'

        switch(choix) {
            case 1:
                printf("Entrez taille de la grille (ex: 7): ");
                scanf("%d", &params->tailleGrille);
                getchar();
                break;
            case 2:
                printf("Entrez forme des pions (ex: rond, carre): ");
                fgets(params->formePions, sizeof(params->formePions), stdin);
                params->formePions[strcspn(params->formePions, "\n")] = 0;
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
                params->autosave = !params->autosave;
                printf("Autosave %s\n", params->autosave ? "activee" : "désactivee");
                break;
            case 6:
                printf("Retour au menu principal...\n");
                break;
            default:
                printf("Option invalide.\n");
        }
    } while (choix != 6);
}
