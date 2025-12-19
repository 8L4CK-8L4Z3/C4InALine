#include <stdio.h>
#include "config.h"

#define CONFIG_FILE "config.dat"

int loadGlobalConfig(ParametresJeu *p) {
    FILE *f = fopen(CONFIG_FILE, "rb");
    if (!f) return 0;
    
    int result = fread(p, sizeof(ParametresJeu), 1, f);
    fclose(f);
    return result == 1;
}

void saveGlobalConfig(ParametresJeu *p) {
    FILE *f = fopen(CONFIG_FILE, "wb");
    if (f) {
        fwrite(p, sizeof(ParametresJeu), 1, f);
        fclose(f);
    }
}
