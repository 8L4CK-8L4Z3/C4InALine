#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <errno.h>
#include "input.h"

struct termios orig_termios;
int rawModeEnabled = 0;

void disableRawMode() {
    if (rawModeEnabled) {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
        rawModeEnabled = 0;
        // Show cursor
        printf("\033[?25h");
    }
}

void enableRawMode() {
    if (!rawModeEnabled) {
        tcgetattr(STDIN_FILENO, &orig_termios);
        atexit(disableRawMode);
        
        struct termios raw = orig_termios;
        raw.c_lflag &= ~(ECHO | ICANON); // Disable echo and canonical mode
        raw.c_cc[VMIN] = 0;
        raw.c_cc[VTIME] = 1; // 100ms timeout
        
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
        rawModeEnabled = 1;
        // Hide cursor
        printf("\033[?25l");
    }
}

int readKey() {
    int nread;
    char c;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if (nread == -1 && errno != EAGAIN) return KEY_NONE;
        // Non-blocking read loop (due to VTIME/VMIN settings or just return NONE if nothing)
        // With VMIN=0, VTIME=1, read returns 0 if timeout.
        if (nread == 0) return KEY_NONE; 
    }

    if (c == '\x1b') {
        char seq[3];
        if (read(STDIN_FILENO, &seq[0], 1) != 1) return KEY_ESC;
        if (read(STDIN_FILENO, &seq[1], 1) != 1) return KEY_ESC;

        if (seq[0] == '[') {
            switch (seq[1]) {
                case 'A': return KEY_UP;
                case 'B': return KEY_DOWN;
                case 'C': return KEY_RIGHT;
                case 'D': return KEY_LEFT;
            }
        }
        return KEY_ESC;
    } else if (c == 127) {
        return KEY_BACKSPACE;
    } else {
        return (int)c;
    }
}
