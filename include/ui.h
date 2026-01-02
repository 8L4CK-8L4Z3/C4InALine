#ifndef UI_H
#define UI_H

void clearScreen();
void printCentered(const char *format, ...);
void printCenteredPrompt(const char *format, ...);
int getTerminalWidth();
int getTerminalHeight();
void printLogo();
void setupConsole(); // Windows setup

// Buffered rendering functions
void ui_begin_draw();
void ui_printf(const char *format, ...);
void ui_end_draw();

#endif // UI_H
