#include <stdio.h>
#include "config.h"

#define CONFIG_FILE "config.dat"

int chargerConfig(ParametresJeu *params) {
    FILE *f = fopen(CONFIG_FILE, "rb");
    if (f) {
        if (fread(params, sizeof(ParametresJeu), 1, f) == 1) {
            fclose(f);
            return 1; // Success
        }
        fclose(f);
    }
    return 0; // Failed
}

void sauvegarderConfig(ParametresJeu *params) {
    FILE *f = fopen(CONFIG_FILE, "wb");
    if (f) {
        fwrite(params, sizeof(ParametresJeu), 1, f);
        fclose(f);
        // printf("Configuration sauvegardee.\n"); // Keep it silent or UI friendly
    }
}
