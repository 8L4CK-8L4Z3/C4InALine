#ifndef UI_H
#define UI_H

void printLogo();
// Draws a box. title can be NULL.
void drawBox(int x, int y, int w, int h, const char *title);
void setCursor(int x, int y);
void clearScreen();

#endif
