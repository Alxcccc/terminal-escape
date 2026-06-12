# Terminal Escape

Terminal-based maze escape game for Windows. Navigate a 10x10 grid to reach the exit while avoiding enemies and walls.

## Controls

- **W** / **A** / **S** / **D**: Move player (P) up / left / down / right
- **ESC**: Quit game

## Build & Run (MinGW/gcc)

```bash
gcc terminalEscape.c -o terminalEscape.exe
./terminalEscape.exe
```

## Game Mechanics

- **Player (P)**: Starts at position (9, 5) - bottom center
- **Exit (E)**: Located at (0, 0) - top-left corner. Reach it to win.
- **Wall (W)**: Blocks movement at (3, 4)
- **Enemy (O)**: At (3, 9). Collision respawns player at starting position.
- **Grid**: 10x10, boundaries enforced

## Code Structure

### Structs

| Struct | Fields | Purpose |
|--------|--------|---------|
| `Map` | `char matrix[10][10]`, `size_t rows`, `size_t columns` | Game grid representation |
| `Object` | `const char token`, `int x`, `int y` | Game entities (player, wall, enemy, exit) |

### Functions

| Function | Signature | Description |
|----------|-----------|-------------|
| `map_init` | `void map_init(Map* m)` | Initializes map grid with walls (`#`) |
| `showMap` | `void showMap(Map m)` | Renders current grid state to terminal |
| `moveObject` | `void moveObject(Object* player, int deltaX, int deltaY, Object wall, Object enemy, int spawn[2], Map* m)` | Universal movement handler with boundary, wall, and enemy collision checks |

### Game Loop (`main`)

1. Initialize map and place objects (player, wall, enemy, exit)
2. `while(1)` loop:
   - Check win condition (player at exit)
   - Check ESC key for exit
   - Poll WASD keys via `GetAsyncKeyState()` → call `moveObject()`
   - Clear screen (`system("cls")`), render map
   - `Sleep(500)` for ~2 FPS frame rate
3. Win message on exit reach, clean exit on ESC

### Movement Logic (`moveObject`)

1. Calculate target position (`nextX`, `nextY`)
2. **Boundary check**: Abort if outside 0-9 range
3. **Wall check**: Abort if target matches wall position
4. **Enemy check**: If target matches enemy → clear old position, reset player to spawn coordinates, redraw at spawn, abort
5. **Normal move**: Draw player at new position, leave trail (`#`), update player coordinates

## Dependencies

- Windows API (`windows.h`) for `GetAsyncKeyState`, `Sleep`
- Standard C: `stdio.h`, `stdbool.h`, `string.h`
- Compiler: MinGW/gcc (tested on Windows)