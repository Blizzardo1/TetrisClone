# TetrisClone

A Tetris clone written in C using SDL3, SDL3_image, and SDL3_ttf.

## Status

### Done
- Window and renderer setup with resizable window, viewport locked to a 16:9 aspect ratio on resize
- All seven tetromino shapes (J, L, I, O, S, Z, T) defined with their standard colors
- Random piece generation
- Piece movement (left/right/soft drop) and rotation
- Hard drop (drops the piece to the lowest available row)
- Tetromino rendering with per-cell fill and outline
- Next-piece preview panel
- Board border rendering
- Font loading and on-screen text rendering (JetBrains Mono Nerd Font)
- Verbose SDL event logging for input/window/mouse events

### Not done
- Pieces never actually lock into the board — nothing currently calls the lock function after a piece lands
- Collision bounds are inconsistent: the board width is reused both as a column count and as a pixel width, so collision checks break after any window resize
- Board clearing only wipes a 4x4 area instead of the full board
- Hold piece (`C` key exists in the input map but does nothing)
- Line clearing and scoring
- Game-over detection
- Level/speed progression
- RNG isn't seeded (the seeding function exists but is never called), so the piece sequence is the same on every run
- Sound/music

### Planned
- Move tetromino-specific logic out of `tetris.c` and into `piece.c`/`piece.h` (currently empty placeholders)
- Split `handle_input` into the individual `handle_hard_drop`, `handle_soft_drop`, `handle_hold`, etc. functions already declared in `input_handler.h`

## Requirements

- CMake 3.15+
- A C17-capable compiler
- SDL3
- SDL3_image
- SDL3_ttf
- pthread

## Compiling

This project uses CMake. `CMakeLists.txt` currently points at local, hardcoded SDL3 build paths — update `CMAKE_PREFIX_PATH` and the include directories in `CMakeLists.txt` (or pass them on the command line) to match where SDL3, SDL3_image, and SDL3_ttf are built on your machine.

```bash
mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH="/path/to/SDL3/build;/path/to/SDL3_image/build;/path/to/SDL3_ttf/build"
cmake --build .
```

This produces a `Tetris` executable in the build directory, alongside the bundled font file it needs at runtime.

## Controls

| Key    | Action     |
|--------|------------|
| ←/→    | Move       |
| ↓      | Soft drop  |
| ↑      | Rotate     |
| Space  | Hard drop  |
| C      | Hold (not yet implemented) |
| Esc    | Quit       |

## Contributing

Contributions are welcome. If you'd like to help:

1. Fork the repo and create a branch for your change
2. Keep changes focused — one fix or feature per PR
3. Match the existing code style (SDL3 idioms, snake_case function names)
4. Open a PR with a short description of what changed and why

Given the project's early state, fixes to the items listed under "Not done" above are especially welcome.

## License

GPL-3.0. See [LICENSE](LICENSE) for details.
