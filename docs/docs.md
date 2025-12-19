# Documentation of Functions

This document lists all functions in the repository, describing their inputs, outputs, and purpose.

## src/ai.c

### `calculerCoupOrdi`
*   **Input**: `char **grille` (game board), `int rows`, `int cols`, `char symboleOrdi`, `char symboleJoueur`, `int difficulty`
*   **Output**: `int` (column index)
*   **Function**: Calculates the best move for the computer based on the difficulty level. Level 1 is random, Level 2 blocks/wins immediately, Level 3 uses Minimax.

### `simulateWin` (static)
*   **Input**: `char **grille`, `int col`, `int rows`, `int cols`, `char symbol`
*   **Output**: `int` (1 if win, 0 otherwise)
*   **Function**: Temporarily places a token to check if it results in a win, then reverts the change.

### `minimax` (static)
*   **Input**: `char **grille`, `int depth`, `int alpha`, `int beta`, `int maximizingPlayer`, `int rows`, `int cols`, `char symboleOrdi`, `char symboleJoueur`
*   **Output**: `int` (score)
*   **Function**: Recursive Minimax algorithm with Alpha-Beta pruning to evaluate board states.

## src/autosave.c

### `sauvegarderPartieSilencieuse`
*   **Input**: `char **grille`, `int rows`, `int cols`, `int joueurCourant`, `ParametresJeu *params`, `int *moves`, `int moveCount`
*   **Output**: `void`
*   **Function**: Saves the current game state to `autosave.dat` without user interaction or console output.

## src/config.c

### `chargerConfig`
*   **Input**: `ParametresJeu *params`
*   **Output**: `int` (1 on success, 0 on failure)
*   **Function**: Loads game configuration from `config.dat` into the provided structure.

### `sauvegarderConfig`
*   **Input**: `ParametresJeu *params`
*   **Output**: `void`
*   **Function**: Saves the current game configuration to `config.dat`.

## src/game.c

### `jouerTour`
*   **Input**: `int joueur`, `char **grille`, `int rows`, `int cols`, `ParametresJeu *params`, `int *playedCol`
*   **Output**: `int` (1 success, 0 invalid/retry, -99 save requested, -2 timeout)
*   **Function**: Manages a single turn for a player (human or AI). Handles input, move validation, timeouts, and drawing the cursor.

### `verifierVictoire`
*   **Input**: `char **grille`, `int rows`, `int cols`, `char symbole`
*   **Output**: `int` (1 if win, 0 otherwise)
*   **Function**: Wrapper for `verifierGrille` to check if the specified symbol has won.

### `grillePleine` (static)
*   **Input**: `char **grille`, `int rows`, `int cols`
*   **Output**: `int` (1 if full, 0 otherwise)
*   **Function**: Checks if the entire grid is full (draw condition).

### `jouerPartie`
*   **Input**: `ParametresJeu *params`, `PartieSauvegardee *saveToResume`
*   **Output**: `void`
*   **Function**: Manages the main game loop, including turn alternation, win/draw detection, autosave, and replay recording. Can resume a saved game.

### `viderBuffer` (static)
*   **Input**: None
*   **Output**: `void`
*   **Function**: Clears the standard input buffer.

## src/grid.c

### `creerGrille`
*   **Input**: `int rows`, `int cols`
*   **Output**: `char**` (pointer to the 2D grid)
*   **Function**: Allocates memory for a 2D character grid and initializes it with empty cells (`.`).

### `libererGrille`
*   **Input**: `char **grille`, `int rows`
*   **Output**: `void`
*   **Function**: Frees the memory allocated for the grid.

### `getAnsiColor` (static)
*   **Input**: `int c` (color ID)
*   **Output**: `const char*` (ANSI escape code)
*   **Function**: Maps an integer color ID to an ANSI color string.

### `afficherGrille`
*   **Input**: `char **grille`, `int rows`, `int cols`, `ParametresJeu *params`
*   **Output**: `void`
*   **Function**: Prints the game grid to the console with proper formatting, colors, and centering.

### `colonnePleine`
*   **Input**: `char **grille`, `int col`, `int rows`
*   **Output**: `int` (1 if full, 0 otherwise)
*   **Function**: Checks if a specific column in the grid is full.

### `insererPion`
*   **Input**: `char **grille`, `int col`, `char symbole`, `int rows`, `int cols`
*   **Output**: `int` (1 if successful, 0 if column invalid or full)
*   **Function**: Inserts a token into the lowest available slot in the specified column.

### `verifierGrille`
*   **Input**: `char **grille`, `int rows`, `int cols`, `char symbole`
*   **Output**: `int` (1 if win found, 0 otherwise)
*   **Function**: Checks for 4 connected symbols horizontally, vertically, or diagonally.

## src/input.c

### `configureTerminal`
*   **Input**: None
*   **Output**: `void`
*   **Function**: Sets the terminal to raw mode (disables echo and canonical mode) for immediate key reading.

### `restoreTerminal`
*   **Input**: None
*   **Output**: `void`
*   **Function**: Restores the terminal to its original settings.

### `readKey`
*   **Input**: None
*   **Output**: `int` (key code)
*   **Function**: Reads a key from stdin, handling escape sequences for arrow keys.

## src/main.c

### `main`
*   **Input**: None
*   **Output**: `int` (exit status)
*   **Function**: Entry point of the program. Initializes configuration, handles the main menu loop, and dispatches actions based on user choice.

## src/menu.c

### `menuSelection`
*   **Input**: `const char *titre`, `const char *options[]`, `int nbOptions`
*   **Output**: `int` (1-based index of selected option)
*   **Function**: Displays an interactive vertical menu with arrow key navigation and returns the user's selection.

### `afficherMenuInteractif`
*   **Input**: None
*   **Output**: `int` (selected option)
*   **Function**: Displays the main menu using `menuSelection`.

### `afficherMenu`
*   **Input**: None
*   **Output**: `void`
*   **Function**: Legacy function for displaying the menu (currently unused/placeholder).

### `afficherReplays`
*   **Input**: None
*   **Output**: `void`
*   **Function**: Placeholder function for displaying replays (actual logic in `replay.c`).

## src/replay.c

### `chargerReplaysDepuisFichier`
*   **Input**: None
*   **Output**: `void`
*   **Function**: Loads replay data from `replays.dat` into the global memory structure.

### `ecrireReplaysFichier`
*   **Input**: None
*   **Output**: `void`
*   **Function**: Writes the global replay data to `replays.dat`.

### `sauvegarderReplay`
*   **Input**: `int *moves`, `int moveCount`, `int rows`, `int cols`, `ParametresJeu *params`, `int winner`
*   **Output**: `void`
*   **Function**: Creates a new replay entry from the game history and saves it to the file. Handles rotation if max replays reached.

### `afficherListeReplays`
*   **Input**: None
*   **Output**: `void`
*   **Function**: Lists available replays on the screen.

### `visionnerReplay`
*   **Input**: None
*   **Output**: `void`
*   **Function**: Interactive function to select and watch a replay. Replays the game move by move.

## src/save.c

### `chargerSauvegardesDepuisFichier`
*   **Input**: None
*   **Output**: `void`
*   **Function**: Loads saved games from `sauvegardes.dat` into memory.

### `aDesSauvegardes`
*   **Input**: None
*   **Output**: `int` (1 if saves exist, 0 otherwise)
*   **Function**: Checks if there are any saved games available.

### `ecrireSauvegardesFichier`
*   **Input**: None
*   **Output**: `void`
*   **Function**: Writes the in-memory saved games to `sauvegardes.dat`.

### `afficherSauvegardes`
*   **Input**: None
*   **Output**: `void`
*   **Function**: Displays a list of saved games (used for debugging or simple listing).

### `chargerPartie`
*   **Input**: `ParametresJeu *currentParams`
*   **Output**: `void`
*   **Function**: Interactive function to select a saved game and resume it. Updates current parameters.

### `sauvegarderPartie`
*   **Input**: `char **grille`, `int rows`, `int cols`, `int joueurCourant`, `ParametresJeu *params`, `int *moves`, `int moveCount`
*   **Output**: `void`
*   **Function**: Interactively prompts for a save name and saves the current game state to the list of saves.

## src/settings.c

### `choisirCouleur`
*   **Input**: `int *colorRef`, `int playerNum`
*   **Output**: `void`
*   **Function**: Interactive menu to select a color for a player. Updates the referenced color variable.

### `getColorName`
*   **Input**: `int c`
*   **Output**: `const char*`
*   **Function**: Returns the string name corresponding to a color ID.

### `gererParametres`
*   **Input**: `ParametresJeu *params`
*   **Output**: `void`
*   **Function**: Interactive menu to view and modify game settings (grid size, symbols, colors, timeout, mode, difficulty).

## src/stats.c

### `mettreAJourStats`
*   **Input**: `int winner`, `int tempsDuree`
*   **Output**: `void`
*   **Function**: Updates the global statistics (games played, wins, time) in `stats.dat`.

### `afficherStats`
*   **Input**: None
*   **Output**: `void`
*   **Function**: Reads and displays the global statistics.

## src/ui.c

### `clearScreen`
*   **Input**: None
*   **Output**: `void`
*   **Function**: Clears the terminal screen using ANSI escape codes.

### `getTerminalWidth`
*   **Input**: None
*   **Output**: `int` (number of columns)
*   **Function**: Returns the current width of the terminal window.

### `getTerminalHeight`
*   **Input**: None
*   **Output**: `int` (number of rows)
*   **Function**: Returns the current height of the terminal window.

### `visibleLength` (static)
*   **Input**: `const char *s`
*   **Output**: `int`
*   **Function**: Calculates the visible length of a string, ignoring ANSI escape codes.

### `printCentered`
*   **Input**: `const char *format`, ...
*   **Output**: `void`
*   **Function**: Prints a formatted string centered horizontally in the terminal. Handles multi-line strings.

### `printCenteredPrompt`
*   **Input**: `const char *format`, ...
*   **Output**: `void`
*   **Function**: Prints a formatted string centered horizontally without a trailing newline (useful for prompts).

### `printLogo`
*   **Input**: None
*   **Output**: `void`
*   **Function**: Prints the game logo centered and in color.
