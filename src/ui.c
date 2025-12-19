#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdarg.h>

void clearScreen() {
    printf("\033[H\033[J");
}

int getTerminalWidth() {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        return w.ws_col;
    }
    return 80; // Default fallback
}

// Helper to calculate visible length ignoring ANSI codes
static int visibleLength(const char *s) {
    int len = 0;
    int inAnsi = 0;
    for (int i = 0; s[i]; i++) {
        if (s[i] == '\033') {
            inAnsi = 1;
            continue; // Skip the ESC
        }
        if (inAnsi) {
            if (s[i] == 'm') inAnsi = 0;
            else if (s[i] >= 64 && s[i] <= 126) {
                 if (strchr("@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~", s[i])) {
                     inAnsi = 0;
                 }
            }
            continue;
        }
        len++;
    }
    return len;
}

void printCentered(const char *format, ...) {
    char buffer[2048];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    char *start = buffer;
    char *end = buffer;
    int width = getTerminalWidth();
    
    while (*start || start == buffer) { // Handle at least one pass if empty but not NULL? No, format can be ""
        if (!*start) break; 

        end = strchr(start, '\n');
        int isLastLine = (end == NULL);
        
        // Temporarily null-terminate
        if (!isLastLine) {
            *end = '\0';
        }

        int len = visibleLength(start);
        int padding = (width - len) / 2;
        if (padding < 0) padding = 0;
        
        if (len > 0) {
            for (int i = 0; i < padding; i++) putchar(' ');
            printf("%s\n", start);
        } else {
             // Empty line (just newline in original string)
             printf("\n");
        }

        if (isLastLine) break;
        start = end + 1;
    }
}

void printLogo() {
    const char *logo[] = {
        "  _____       _                                  _  _   ",
        " |  __ \\     (_)                                | || |  ",
        " | |__) |   _ _ ___ ___  __ _ _ __   ___ ___    | || |_ ",
        " |  ___/ | | | / __/ __|/ _` | '_ \\ / __/ _ \\   |__   _|",
        " | |   | |_| | \\__ \\__ \\ (_| | | | | (_|  __/      | |  ",
        " |_|    \\__,_|_|___/___/\\__,_|_| |_|\\___\\___|      |_|  ",
        NULL
    };
    printf("\n\033[1;36m"); // Cyan
    for (int i = 0; logo[i] != NULL; i++) {
        printCentered("%s", logo[i]);
    }
    printf("\033[0m\n");
}
