# Helicopter Combat

A text-based helicopter combat simulator written in C++.

## Description

Helicopter Combat is a console-based game that simulates helicopter combat missions.

## Features

- Text-based helicopter flight simulation
- Combat mechanics with various weapons systems
- Multiple enemy types
- Mission-based gameplay

## Prerequisites

- C++ compiler (g++ 9 or later recommended)
- CMake (version 3.16 or higher)
- Make

## Building the Project

### Using CMake

1. Clone the repository:
   ```bash
   git clone https://github.com/mars-rift/Helicopter-Combat.git
   cd Helicopter-Combat
   ```

2. Create a build directory and run CMake:
   ```bash
   mkdir -p build
   cd build
   cmake ..
   ```

3. Build the project:
   ```bash
   make
   ```

### Using VSCode

The project includes VSCode configurations in the .vscode folder. To build using VSCode:

1. Open the project in VSCode
2. Press `Ctrl+Shift+B` or select `Terminal > Run Build Task`
3. Select the C/C++ build task

## Running the Game

After building, you can run the game from the build directory:

```bash
cd build
./HelicopterCombat
```

## Project Structure

```
├── src/                  # Source files
│   ├── Enemy.cpp         # Enemy implementation
│   ├── Enemy.h           # Enemy class definition
│   ├── Game.cpp          # Game logic implementation
│   ├── Game.h            # Game class definition
│   ├── Helicopter.cpp    # Helicopter implementation
│   ├── Helicopter.h      # Helicopter class definition
│   ├── HelicopterCombat.cpp # Main game implementation
│   ├── HelicopterCombat.h   # Main game header
│   ├── main.cpp          # Entry point
│   ├── Weapon.cpp        # Weapon implementation
│   └── Weapon.h          # Weapon class definition
├── CMakeLists.txt        # CMake configuration
└── .vscode/              # VSCode configuration
```

## License

This project is licensed under the MIT License - see the LICENSE file for details.

