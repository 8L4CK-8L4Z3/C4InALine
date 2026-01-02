#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

void setupConsole() {
#ifdef _WIN32
    // Enable ANSI escape codes
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    
    // Set UTF-8 code page just in case
    SetConsoleOutputCP(CP_UTF8);
#endif
}

void clearScreen() {
#ifdef _WIN32
    // Since we enabled ANSI, we can use the same code, 
    // OR use system("cls") if ANSI fails. 
    // Let's stick to ANSI as we enabled it.
    printf("\033[H\033[J");
#else
    printf("\033[H\033[J");
#endif
}

int getTerminalWidth() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    return 80;
#else
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        return w.ws_col;
    }
    return 80; // Default fallback
#endif
}

int getTerminalHeight() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
    return 24;
#else
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        return w.ws_row;
    }
    return 24; // Default fallback
#endif
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

void printCenteredPrompt(const char *format, ...) {
    char buffer[2048];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    int width = getTerminalWidth();
    int len = visibleLength(buffer);
    int padding = (width - len) / 2;
    if (padding < 0) padding = 0;
    
    for (int i = 0; i < padding; i++) putchar(' ');
    printf("%s", buffer);
    fflush(stdout);
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
