# TuiSweeper

A fast, lightweight Minesweeper clone for the terminal, written in modern C++23.

## Features

- **Modern C++**: Built using the latest C++23 standards.
- **TUI Interface**: Pure terminal-based gameplay using standard POSIX terminal
  controls.
- **Difficulty Levels**:
  - **Beginner**: 9x9 board with 10 mines.
  - **Intermediate**: 16x16 board with 40 mines.
  - **Expert**: 30x16 board with 99 mines.
- **Customizable**: Logic separated into a modular `Board` class and a `Game`
  orchestration layer.

## Controls

| Key                | Action      |
| :----------------- | :---------- |
| `W`, `A`, `S`, `D` | Move cursor |
| `Space`            | Reveal cell |
| `F`                | Toggle flag |
| `Q`                | Quit game   |

## Prerequisites

- **C++23 Compiler**: (e.g., GCC 13+ or Clang 16+)
- **CMake**: 3.31 or higher
- **Nix** (Optional): A `flake.nix` is provided for a reproducible development
  environment.

## Building

### Using Nix (Recommended)

If you have Nix installed with Flakes enabled, you can enter the development
shell or run the project directly:

```bash
nix develop
# Then follow the CMake build steps below, or:
nix run
```

### Manual Build

1. Create a build directory:

   ```bash
   mkdir build && cd build
   ```

2. Configure the project:

   ```bash
   cmake ..
   ```

3. Compile:

   ```bash
   make
   ```

## Running

After building, you can run the executable from the `build` directory:

```bash
./tsweep
```

## Architecture

- **`Board`**: Manages the grid state, mine generation, and cell revealing
  logic.
- **`TerminalHandler`**: Manages terminal state, raw mode, and screen clearing.
- **`Game`**: Orchestrates the game loop, input processing, and rendering.
