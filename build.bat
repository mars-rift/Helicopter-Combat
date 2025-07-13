@echo off
echo Building Helicopter Combat...

REM Create build directory
if exist build-release rmdir /s /q build-release
mkdir build-release
cd build-release

REM Configure with CMake
echo Configuring project...
cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release ..
if errorlevel 1 (
    echo CMake configuration failed!
    cd ..
    pause
    exit /b 1
)

REM Build the project
echo Building project...
cmake --build . --config Release
if errorlevel 1 (
    echo Build failed!
    cd ..
    pause
    exit /b 1
)

REM Copy executable to root
if exist Release\HelicopterCombat.exe (
    copy Release\HelicopterCombat.exe ..\HelicopterCombat.exe
    echo Build successful! Executable created: HelicopterCombat.exe
) else (
    echo Executable not found!
)

cd ..
echo Build process completed!
pause
