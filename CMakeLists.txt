cmake_minimum_required(VERSION 3.10)

# Set the project name
project(HelicopterCombat)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the executable
add_executable(HelicopterCombat 
    src/main.cpp
    src/Game.cpp
    src/Helicopter.cpp
    src/Weapon.cpp
    src/Enemy.cpp
    src/HelicopterCombat.cpp
    src/Mission.cpp
    src/Environment.cpp
)

# Include directories
target_include_directories(HelicopterCombat PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Enable all warnings and treat them as errors for better code quality
target_compile_options(HelicopterCombat PRIVATE -Wall -Wextra)

# Link math library if needed
target_link_libraries(HelicopterCombat m)
