// ... Début du fichier save.c

#include <stdio.h>
#include <string.h>
#include "commun.h"

PartieSauvegardee sauvegardes[MAX_SAVED_GAMES];
int nbSauvegardes = 0;

// Affiche les parties sauvegardées
void afficherSauvegardes() {
    if (nbSauvegardes == 0) {
        printf("Aucune sauvegarde disponible.\n");
    } else {
        printf("\n=== Sauvegardes ===\n");
        for (int i = 0; i < nbSauvegardes; i++) {
            printf("%d. %s\n", i+1, sauvegardes[i].nomPartie);
        }
    }
}

// Charge une partie sauvegardée
void chargerPartie() {
    if (nbSauvegardes == 0) {
        printf("Aucune sauvegarde a charger.\n");
        return;
    }
    afficherSauvegardes();
    int choix;
    printf("Choisissez le numero de la partie a charger : ");
    scanf("%d", &choix);
    getchar();
    if (choix < 1 || choix > nbSauvegardes) {
        printf("Numéro invalide.\n");
        return;
    }
    printf("Partie '%s' chargee avec succes !\n", sauvegardes[choix-1].nomPartie);
    // Ici vous pouvez utiliser sauvegardes[choix-1] pour initialiser la partie
}

// Sauvegarde une partie
void sauvegarderPartie() {
    if (nbSauvegardes >= MAX_SAVED_GAMES) {
        printf("Limite de sauvegardes atteinte.\n");
        return;
    }
    PartieSauvegardee nouvellePartie;
    printf("Entrez un nom pour la sauvegarde : ");
    fgets(nouvellePartie.nomPartie, sizeof(nouvellePartie.nomPartie), stdin);
    nouvellePartie.nomPartie[strcspn(nouvellePartie.nomPartie, "\n")] = 0;

    // Valeurs standards pour l'exemple
    nouvellePartie.parametres.tailleGrille = 7;
    strcpy(nouvellePartie.parametres.formePions, "rond");
    strcpy(nouvellePartie.parametres.couleurPions, "rouge");
    nouvellePartie.parametres.tempsLimite = 30;
    nouvellePartie.parametres.autosave = 1;

    sauvegardes[nbSauvegardes++] = nouvellePartie;
    printf("Partie sauvegardée sous le nom : %s\n", nouvellePartie.nomPartie);
}
