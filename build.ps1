# PowerShell build script for Helicopter Combat
# This script builds the project to a single Windows executable

param(
    [string]$BuildType = "Release",
    [string]$Architecture = "x64"
)

Write-Host "Building Helicopter Combat - $BuildType configuration for $Architecture" -ForegroundColor Green

# Create build directory
$BuildDir = "build-$Architecture-$BuildType"
if (Test-Path $BuildDir) {
    Write-Host "Cleaning existing build directory..." -ForegroundColor Yellow
    Remove-Item -Recurse -Force $BuildDir
}
New-Item -ItemType Directory -Path $BuildDir | Out-Null

# Configure with CMake
Write-Host "Configuring project with CMake..." -ForegroundColor Cyan
Set-Location $BuildDir

$ConfigureCommand = "cmake -G `"Visual Studio 17 2022`" -A $Architecture -DCMAKE_BUILD_TYPE=$BuildType .."
if ($Architecture -eq "x64") {
    $ConfigureCommand = "cmake -G `"Visual Studio 17 2022`" -A x64 -DCMAKE_BUILD_TYPE=$BuildType .."
} elseif ($Architecture -eq "Win32") {
    $ConfigureCommand = "cmake -G `"Visual Studio 17 2022`" -A Win32 -DCMAKE_BUILD_TYPE=$BuildType .."
}

Invoke-Expression $ConfigureCommand

if ($LASTEXITCODE -ne 0) {
    Write-Host "CMake configuration failed!" -ForegroundColor Red
    Set-Location ..
    exit 1
}

# Build the project
Write-Host "Building project..." -ForegroundColor Cyan
cmake --build . --config $BuildType

if ($LASTEXITCODE -ne 0) {
    Write-Host "Build failed!" -ForegroundColor Red
    Set-Location ..
    exit 1
}

# Copy the executable to the root directory
$ExeName = "HelicopterCombat.exe"
$SourcePath = "$BuildType\$ExeName"
$DestPath = "..\$ExeName"

if (Test-Path $SourcePath) {
    Copy-Item $SourcePath $DestPath -Force
    Write-Host "Build successful! Executable created: $ExeName" -ForegroundColor Green
    
    # Display file size
    $FileInfo = Get-Item $DestPath
    $FileSizeMB = [math]::Round($FileInfo.Length / 1MB, 2)
    Write-Host "File size: $FileSizeMB MB" -ForegroundColor Cyan
} else {
    Write-Host "Executable not found at expected location: $SourcePath" -ForegroundColor Red
}

Set-Location ..

Write-Host "Build process completed!" -ForegroundColor Green
