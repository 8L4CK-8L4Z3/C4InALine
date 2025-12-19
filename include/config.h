#ifndef CONFIG_H
#define CONFIG_H

#include "commun.h"

// Load settings from config.dat. Returns 1 if success, 0 if not found/error.
int loadGlobalConfig(ParametresJeu *p);

// Save settings to config.dat
void saveGlobalConfig(ParametresJeu *p);

#endif
