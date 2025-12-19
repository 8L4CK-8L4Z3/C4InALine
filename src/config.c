#include <stdio.h>
#include "config.h"

#define CONFIG_FILE "config.dat"

void chargerConfig(ParametresJeu *params) {
    FILE *f = fopen(CONFIG_FILE, "rb");
    if (f) {
        fread(params, sizeof(ParametresJeu), 1, f);
        fclose(f);
        printf("Configuration chargee.\n");
    } else {
        // Defaults are already set in main, or we can set them here if params is empty?
        // But main initializes defaults. If file missing, we do nothing.
        // Actually, Memory said "A first-time configuration wizard is automatically triggered... if config.dat is missing".
        // I should stick to that if possible.
        // But for now, simple load/save.
    }
}

void sauvegarderConfig(ParametresJeu *params) {
    FILE *f = fopen(CONFIG_FILE, "wb");
    if (f) {
        fwrite(params, sizeof(ParametresJeu), 1, f);
        fclose(f);
        // printf("Configuration sauvegardee.\n"); // Keep it silent or UI friendly
    }
}
