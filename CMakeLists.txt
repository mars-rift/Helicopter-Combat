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
    src/HelicopterCombat.cpp
)

# Include directories
target_include_directories(HelicopterCombat PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
