cmake_minimum_required(VERSION 3.10)

# Set the project name
project(HelicopterCombat)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# For static linking on Windows
if(WIN32)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /INCREMENTAL:NO")
endif()

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
target_compile_options(HelicopterCombat PRIVATE 
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra>
)

# For creating a single executable on Windows
if(WIN32 AND MSVC)
    # Static linking for MSVC runtime
    target_compile_options(HelicopterCombat PRIVATE 
        $<$<CONFIG:Release>:/MT>
        $<$<CONFIG:Debug>:/MTd>
    )
    
    # Link statically
    set_target_properties(HelicopterCombat PROPERTIES
        LINK_FLAGS_RELEASE "/INCREMENTAL:NO"
        LINK_FLAGS_DEBUG "/INCREMENTAL:NO"
    )
endif()

# Link math library if needed (for MinGW/GCC)
if(MINGW OR (CMAKE_CXX_COMPILER_ID STREQUAL "GNU"))
    target_link_libraries(HelicopterCombat -static-libgcc -static-libstdc++ -static)
endif()
