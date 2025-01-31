cmake_minimum_required(VERSION 3.10)

# Set the project name
project(HelicopterCombat)

# Add the executable
add_executable(HelicopterCombat 
    src/main.cpp
    src/Game.cpp
    src/Helicopter.cpp
    src/Weapon.cpp
    src/Enemy.cpp
    src/Game.h
    src/Helicopter.h
    src/Weapon.h
    src/Enemy.h
)
