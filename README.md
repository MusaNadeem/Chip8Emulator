# Chip8Emulator

A lightweight CHIP-8 emulator written in C++.

## Features
- Standard CHIP-8 instruction set support.
- Fully executable test ROMs included.

## Build Instructions

This project uses CMake. Ensure you have CMake and a C++ compiler installed on your system.

```bash
# Create a build directory
mkdir build
cd build

# Configure the project
cmake ..

# Compile
make
```

## Usage

After compiling, run the emulator by passing a CHIP-8 ROM as an argument:

```bash
./build/Chip8Emulator path/to/rom.ch8
```

Example using included ROMs:

```bash
./build/Chip8Emulator roms/1-chip8-logo.ch8
./build/Chip8Emulator roms/SpaceInvaders.ch8
```

There are several ROMs available in the `roms/` directory, including:
- `1-chip8-logo.ch8`
- `15Puzzle.ch8`
- `SpaceInvaders.ch8`
- `AstroDodge.ch8`

## Project Structure
- `src/`: Source code (`main.cpp`, `Chip8.cpp`)
- `include/`: Header files (`Chip8.hpp`)
- `roms/`: CHIP-8 ROMs and test files.
