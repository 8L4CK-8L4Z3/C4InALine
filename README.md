# Puissance 4 (Connect 4)

A Connect 4 game implementation in C with a Text User Interface (TUI).
Supports Player vs Player and Player vs Computer (AI with multiple difficulties).

## Features
- Interactive Menu
- Save/Load Game
- Replay System
- Statistics
- Customizable Settings (Board size, colors, symbols, difficulty)
- Cross-platform (Linux/macOS/Windows)

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
3. Compile using GCC (ensure `src` files are included):
   ```cmd
   gcc -o game.exe src/*.c -Iinclude
   ```
   *Note: If you have `make` installed (e.g., via MinGW/MSYS), you can also run `make`.*
4. Run the game:
   ```cmd
   game.exe
   ```

## Controls
- **Arrow Keys (Left/Right):** Select column / Navigate menus
- **Arrow Keys (Up/Down):** Navigate menus
- **Enter:** Confirm selection / Drop token
- **S:** Save game during play
