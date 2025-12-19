#ifndef INPUT_H
#define INPUT_H

// Key codes
enum Key {
    KEY_NONE = 0,
    KEY_ENTER = 10,
    KEY_ESC = 27,
    KEY_UP = 1001,
    KEY_DOWN = 1002,
    KEY_LEFT = 1003,
    KEY_RIGHT = 1004,
    KEY_BACKSPACE = 127
};

void enableRawMode();
void disableRawMode();
int readKey();

#endif
