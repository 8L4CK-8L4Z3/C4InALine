#ifndef UI_H
#define UI_H

void clearScreen();
void printCentered(const char *format, ...);
void printCenteredPrompt(const char *format, ...);
int getTerminalWidth();
int getTerminalHeight();
void printLogo();

#endif // UI_H
