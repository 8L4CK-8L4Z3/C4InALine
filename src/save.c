#include <stdio.h>
#include <string.h>
#include "commun.h"
#include "game.h" // For jouerPartie prototype
#include "ui.h"

#define SAVE_FILE "sauvegardes.dat"

// Global/Static array to hold saves in memory (could be just one read from file)
PartieSauvegardee sauvegardes[MAX_SAVED_GAMES];
int nbSauvegardes = 0;

void chargerSauvegardesDepuisFichier() {
    FILE *f = fopen(SAVE_FILE, "rb");
    if (f) {
        nbSauvegardes = fread(sauvegardes, sizeof(PartieSauvegardee), MAX_SAVED_GAMES, f);
        fclose(f);
    } else {
        nbSauvegardes = 0;
    }
}

void ecrireSauvegardesFichier() {
    FILE *f = fopen(SAVE_FILE, "wb");
    if (f) {
        fwrite(sauvegardes, sizeof(PartieSauvegardee), nbSauvegardes, f);
        fclose(f);
    } else {
        perror("Erreur sauvegarde fichier");
    }
}

// Affiche les parties sauvegardées
void afficherSauvegardes() {
    chargerSauvegardesDepuisFichier();
    if (nbSauvegardes == 0) {
        printCentered("Aucune sauvegarde disponible.");
    } else {
        printCentered("\n=== Sauvegardes ===");
        char buf[200];
        for (int i = 0; i < nbSauvegardes; i++) {
            snprintf(buf, sizeof(buf), "%d. %s (Grille %dx%d, Joueur %d)", 
                   i+1, 
                   sauvegardes[i].nomPartie, 
                   sauvegardes[i].parametres.tailleGrille,
                   sauvegardes[i].parametres.tailleGrille,
                   sauvegardes[i].joueurCourant);
            printCentered(buf);
        }
    }
}

// Charge une partie sauvegardée
void chargerPartie(ParametresJeu *currentParams) {
    chargerSauvegardesDepuisFichier();
    if (nbSauvegardes == 0) {
        printCentered("Aucune sauvegarde a charger.");
        return;
    }
    afficherSauvegardes();
    int choix;
    printCentered("Choisissez le numero de la partie a charger : ");
    scanf("%d", &choix);
    getchar();
    if (choix < 1 || choix > nbSauvegardes) {
        printf("Numéro invalide.\n");
        return;
    }
    
    PartieSauvegardee *save = &sauvegardes[choix-1];
    printf("Partie '%s' chargee avec succes !\n", save->nomPartie);
    
    // Resume game
    *currentParams = save->parametres; // Update global params context if needed
    jouerPartie(&save->parametres, save);
}

// Sauvegarde une partie
void sauvegarderPartie(char **grille, int rows, int cols, int joueurCourant, ParametresJeu *params, int *moves, int moveCount) {
    chargerSauvegardesDepuisFichier();
    
    if (nbSauvegardes >= MAX_SAVED_GAMES) {
        // Overwrite oldest? Or just fail. Let's fail for now.
        printf("Limite de sauvegardes atteinte (max 10). Suppression de la plus ancienne...\n");
        // Shift left
        for(int i=0; i<nbSauvegardes-1; i++) {
            sauvegardes[i] = sauvegardes[i+1];
        }
        nbSauvegardes--;
    }

    PartieSauvegardee nouvellePartie;
    printf("Entrez un nom pour la sauvegarde : ");
    // Clear buffer if needed?
    // Clear buffer properly
    // The -99 input was via scanf("%d"), leaving newline in buffer.
    // We need to consume until newline.
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    fgets(nouvellePartie.nomPartie, sizeof(nouvellePartie.nomPartie), stdin);
    nouvellePartie.nomPartie[strcspn(nouvellePartie.nomPartie, "\n")] = 0;

    nouvellePartie.parametres = *params;
    nouvellePartie.rows = rows;
    nouvellePartie.cols = cols;
    nouvellePartie.joueurCourant = joueurCourant;
    
    // Copy moves
    if (moveCount <= MAX_MOVES_TOTAL) {
        memcpy(nouvellePartie.moves, moves, moveCount * sizeof(int));
        nouvellePartie.moveCount = moveCount;
    } else {
        nouvellePartie.moveCount = 0; // Error case
    }

    // Serialize grid
    // Ensure rows/cols fit MAX
    if (rows > MAX_ROWS || cols > MAX_COLS) {
        printf("Erreur: Grille trop grande pour sauvegarde.\n");
        return;
    }
    
    // Clear
    memset(nouvellePartie.grille, 0, sizeof(nouvellePartie.grille));
    for(int i=0; i<rows; i++) {
        for(int j=0; j<cols; j++) {
            nouvellePartie.grille[i][j] = grille[i][j];
        }
    }

    sauvegardes[nbSauvegardes++] = nouvellePartie;
    ecrireSauvegardesFichier();
    printf("Partie sauvegardée sous le nom : %s\n", nouvellePartie.nomPartie);
}
