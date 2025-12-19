#include <stdio.h>
#include "commun.h"
#include "game.h"

// Prototypes
void afficherMenu();
void gererParametres(ParametresJeu *params);
void afficherSauvegardes();
void chargerPartie();
void sauvegarderPartie();
void afficherReplays();

int main() {
    int choix;
    ParametresJeu parametres = {7, "rond", "rouge", 30, 1};

    do {
        afficherMenu();
        scanf("%d", &choix);
        getchar(); // consommer '\n'

        switch (choix) {
            case 1:
                jouerPartie(parametres.tailleGrille, parametres.tailleGrille);
                break;
            case 2:
                gererParametres(&parametres);
                break;
            case 3: {
                int subChoix;
                do {
                    printf("\n1. Charger une partie\n2. Enregistrer une partie\n0. Retour\nChoix : ");
                    scanf("%d", &subChoix);
                    getchar();
                    switch(subChoix) {
                        case 1:
                            chargerPartie();
                            break;
                        case 2:
                            sauvegarderPartie();
                            break;
                        case 0:
                            break;
                        default:
                            printf("Option invalide.\n");
                    }
                } while (subChoix != 0);
                break;
            }
            case 4:
                afficherReplays();
                break;
            case 5:
                printf("Quitter le jeu. Au revoir !\n");
                break;
            default:
                printf("Choix invalide, veuillez reessayer.\n");
        }
    } while (choix != 5);

    return 0;
}
