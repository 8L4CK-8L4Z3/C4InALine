#include "input.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
static DWORD orig_console_mode;
static int raw_mode_enabled = 0;
#else
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <fcntl.h>
static struct termios orig_termios;
static int raw_mode_enabled = 0;
#endif

void configureTerminal() {
    if (raw_mode_enabled) return;
#ifdef _WIN32
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    if (hIn == INVALID_HANDLE_VALUE) return;
    GetConsoleMode(hIn, &orig_console_mode);
    atexit(restoreTerminal);
    
    // Disable line input and echo input
    DWORD new_mode = orig_console_mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
    SetConsoleMode(hIn, new_mode);
#else
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(restoreTerminal);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON); // Disable echo and canonical mode
    raw.c_cc[VMIN] = 0; // Non-blocking read (we use select)
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
#endif
    raw_mode_enabled = 1;
}

void restoreTerminal() {
    if (!raw_mode_enabled) return;
#ifdef _WIN32
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    if (hIn != INVALID_HANDLE_VALUE) {
        SetConsoleMode(hIn, orig_console_mode);
    }
#else
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
#endif
    raw_mode_enabled = 0;
}

int readKey() {
#ifdef _WIN32
    if (_kbhit()) {
        int c = _getch();
        if (c == 0 || c == 224) { // Special key
            c = _getch();
            switch(c) {
                case 72: return KEY_UP;
                case 80: return KEY_DOWN;
                case 77: return KEY_RIGHT;
                case 75: return KEY_LEFT;
            }
            return 0; // Unknown special
        }
        if (c == 13) return KEY_ENTER;
        if (c == 27) return KEY_ESCAPE;
        return c;
    }
    return 0;
#else
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
#endif
}
