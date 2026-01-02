#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <fcntl.h>

static struct termios orig_termios;
static int raw_mode_enabled = 0;

void configureTerminal() {
    if (raw_mode_enabled) return;
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(restoreTerminal);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON); // Disable echo and canonical mode
    raw.c_cc[VMIN] = 0; // Non-blocking read (we use select)
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    raw_mode_enabled = 1;
}

void restoreTerminal() {
    if (!raw_mode_enabled) return;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    raw_mode_enabled = 0;
}

int readKey() {
    char c;
    int nread = read(STDIN_FILENO, &c, 1);
    if (nread == 0) return 0; // Nothing read
    if (nread == -1) return 0; // Error

    if (c == '\033') { // Escape sequence
        char seq[3];
        if (read(STDIN_FILENO, &seq[0], 1) != 1) return KEY_ESCAPE;
        if (read(STDIN_FILENO, &seq[1], 1) != 1) return KEY_ESCAPE;

        if (seq[0] == '[') {
            switch(seq[1]) {
                case 'A': return KEY_UP;
                case 'B': return KEY_DOWN;
                case 'C': return KEY_RIGHT;
                case 'D': return KEY_LEFT;
            }
        }
        return KEY_ESCAPE;
    }
    return c;
}
