#ifndef INPUT_H
#define INPUT_H

// Key codes
#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_RIGHT 67
#define KEY_LEFT 68
#define KEY_ENTER 10
#define KEY_ESCAPE 27

void configureTerminal();
void restoreTerminal();
int readKey(); // Returns key code or character

#endif
