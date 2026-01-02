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

// Buffering system
#define FRAME_BUFFER_SIZE 65536
static char frameBuffer[FRAME_BUFFER_SIZE];
static int framePos = 0;
static int isBuffering = 0;

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
    // If buffering, we don't clear screen traditionally,
    // we assume the next frame overwrites or we rely on ui_end_draw to clear residue.
    // However, legacy code still calls this.
    // For buffered mode, we want to start at Home.
    if (isBuffering) {
        // Do nothing here, ui_begin_draw handles setup.
        // Or we could append the clear code to buffer.
        // But usually clearScreen calls are followed by print calls.
        // Let's make clearScreen append "Home" + "Clear" to buffer if buffering.
        ui_printf("\033[H\033[J");
    } else {
        printf("\033[H\033[J");
    }
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
            // Reconstruct padding string
            for (int i = 0; i < padding; i++) {
                if(isBuffering) ui_printf(" ");
                else putchar(' ');
            }
            if(isBuffering) ui_printf("%s\n", start);
            else printf("%s\n", start);
        } else {
             // Empty line (just newline in original string)
             if(isBuffering) ui_printf("\n");
             else printf("\n");
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
    
    for (int i = 0; i < padding; i++) {
        if(isBuffering) ui_printf(" ");
        else putchar(' ');
    }
    if(isBuffering) ui_printf("%s", buffer);
    else {
        printf("%s", buffer);
        fflush(stdout);
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
    if(isBuffering) ui_printf("\n\033[1;36m");
    else printf("\n\033[1;36m"); // Cyan

    for (int i = 0; logo[i] != NULL; i++) {
        printCentered("%s", logo[i]);
    }
    if(isBuffering) ui_printf("\033[0m\n");
    else printf("\033[0m\n");
}

// Implement buffering
void ui_begin_draw() {
    isBuffering = 1;
    framePos = 0;
    frameBuffer[0] = '\0';
    // Start with Home cursor
    // We do NOT clear screen here to avoid flicker.
    // We assume we overwrite or clear at end.
    // Actually, to ensure overwrite works, we should start with Home.
    // But we don't want \033[J (Clear Down) yet.
    // However, if the new frame is smaller, artifacts remain.
    // The "Clear Down" should happen AFTER drawing the frame if we want to be clean.
    // But if we use clearScreen() inside the logic, it puts Home+ClearJ into the buffer.
    // Let's rely on ui_end_draw to finalize.
    // We add Home to start.
    ui_printf("\033[H");
}

void ui_printf(const char *format, ...) {
    if (!isBuffering) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        return;
    }

    va_list args;
    va_start(args, format);
    // Append to buffer
    int remaining = FRAME_BUFFER_SIZE - framePos - 1;
    if (remaining > 0) {
        int written = vsnprintf(frameBuffer + framePos, remaining, format, args);
        if (written > 0) {
            framePos += written;
            if (framePos >= FRAME_BUFFER_SIZE) framePos = FRAME_BUFFER_SIZE - 1;
        }
    }
    va_end(args);
}

void ui_end_draw() {
    if (!isBuffering) return;
    isBuffering = 0;

    // Append Clear Down to ensure anything below this frame is wiped
    // This effectively clears the screen but only AFTER we have drawn the new frame in the buffer.
    // Wait, if we put ClearJ at the end of the buffer, it clears everything AFTER the cursor.
    // The cursor is at the end of our print.
    // So yes, this cleans up the rest of the screen.
    if (FRAME_BUFFER_SIZE - framePos > 5) {
        strcat(frameBuffer, "\033[J");
    }

    // Now print everything in one go
    // Note: Use fwrite or printf.
    // printf might have issues with % in buffer if not careful.
    // Use puts? puts appends newline.
    // Use fwrite to stdout.
    fwrite(frameBuffer, 1, strlen(frameBuffer), stdout);
    fflush(stdout);
}
