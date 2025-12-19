#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ui.h"

void clearScreen() {
    printf("\033[2J\033[H");
}

void setCursor(int x, int y) {
    // ANSI uses row;col, so y;x. 1-based.
    printf("\033[%d;%dH", y, x);
}

void printLogo() {
    // Simple ASCII Art
    // Using a simple color
    printf("\033[1;36m"); // Cyan Bold
    printf("    ____        _                                __ __ \n");
    printf("   / __ \\__  __(_)_____________ _____  ________ / // / \n");
    printf("  / /_/ / / / / / ___/ ___/ __ `/ __ \\/ ___/ _ \\/ // /_ \n");
    printf(" / ____/ /_/ / (__  |__  ) /_/ / / / / /__/  __/__  __/ \n");
    printf("/_/    \\__,_/_/____/____/\\__,_/_/ /_/\\___/\\___/  /_/    \n");
    printf("\033[0m\n");
}

void drawBox(int x, int y, int w, int h, const char *title) {
    // Draw box using ASCII or UTF-8 box chars. 
    // Using simple ASCII for safety: + - |
    // Or extended ASCII if locale allows. Let's stick to safe ASCII first or ANSI line drawing if compatible.
    // Let's use standard + - | for maximum compatibility unless requested otherwise.
    
    // Top Border
    setCursor(x, y);
    printf("+");
    for(int i=0; i<w-2; i++) printf("-");
    printf("+");

    // Title
    if (title) {
        setCursor(x + 2, y);
        printf(" %s ", title);
    }

    // Sides
    for(int i=1; i<h-1; i++) {
        setCursor(x, y+i);
        printf("|");
        setCursor(x+w-1, y+i);
        printf("|");
    }

    // Bottom Border
    setCursor(x, y+h-1);
    printf("+");
    for(int i=0; i<w-2; i++) printf("-");
    printf("+");
}
