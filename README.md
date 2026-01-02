# Puissance 4 (Connect 4)

A Connect 4 game implementation in C with a Text User Interface (TUI).
Supports Player vs Player and Player vs Computer (AI with multiple difficulties).

## Features
- **Interactive Menu:** Navigate through options using arrow keys.
- **Save/Load Game:** Save your progress and resume later.
- **Replay System:** Watch replays of past games move-by-move.
- **Statistics:** Track wins, losses, and play time.
- **Customizable Settings:**
    - Board size (4x4 to 20x20)
    - Player symbols and colors
    - AI difficulty (Easy, Medium, Hard)
    - Time limit per turn
- **Cross-platform:** Runs on Linux, macOS, and Windows.

## Compilation and Execution

### Linux / macOS
Requirements: `gcc`, `make`

1. Open a terminal.
2. Navigate to the project root.
3. Run `make` to compile.
   ```bash
   make
   ```
4. Run the game:
   ```bash
   ./game
   ```

### Windows
Requirements: MinGW (GCC) or compatible C compiler.

1. Open Command Prompt or PowerShell.
2. Navigate to the project root.
3. Compile using GCC:
   ```cmd
   gcc -o game.exe src/*.c -Iinclude
   ```
   *Note: If you have `make` installed (e.g., via MinGW/MSYS), you can also run `make`.*
4. Run the game:
   ```cmd
   game.exe
   ```

## Controls
- **Arrow Keys (Left/Right):** Select column in game / Navigate menus (Up/Down for menus as well).
- **Arrow Keys (Up/Down):** Navigate vertical menus.
- **Enter:** Confirm selection / Drop token.
- **S:** Save game during play (when it's your turn).
