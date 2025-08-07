@echo off
setlocal enabledelayedexpansion

:: Configuration
set PHYSX_BUILD_TYPE=checked
set PHYSX_ROOT=%cd%\external\physx
set PHYSX_TMP=%PHYSX_ROOT%\tmp
set PHYSX_LIB_OUTPUT=%PHYSX_ROOT%\lib\windows\%PHYSX_BUILD_TYPE%
set PHYSX_SOURCE_DIR=%PHYSX_TMP%\PhysX\physx

echo [VelixFlow] Cleaning old PhysX build if exists...
rmdir /S /Q "%PHYSX_TMP%" 2>nul
mkdir "%PHYSX_TMP%"
cd "%PHYSX_TMP%"

echo [VelixFlow] Cloning NVIDIA PhysX SDK...
git clone https://github.com/NVIDIA-Omniverse/PhysX.git --depth 1

cd "%PHYSX_SOURCE_DIR%"

echo [VelixFlow] Generating Visual Studio project files...
call generate_projects.bat vc17win64-cpu-only

echo [VelixFlow] Building PhysX in %PHYSX_BUILD_TYPE% mode...
cd "%PHYSX_SOURCE_DIR%\compiler\vc17win64-cpu-only"

msbuild PhysXSDK.sln /p:Configuration=Checked

if errorlevel 1 (
    echo [VelixFlow] Error: Failed to build PhysX
    exit /b 1
)

echo [VelixFlow] Moving built .lib files to %PHYSX_LIB_OUTPUT%...
mkdir "%PHYSX_LIB_OUTPUT%" 2>nul

robocopy "%PHYSX_SOURCE_DIR%\bin\win.x86_64.vc143.mt/%PHYSX_BUILD_TYPE%" "%PHYSX_LIB_OUTPUT%" *.lib /njh /njs /ndl /nc /ns /np >nul

echo [VelixFlow] Cleaning up PhysX source...
cd "%PHYSX_ROOT%"
rmdir /S /Q "%PHYSX_TMP%"

echo [VelixFlow] PhysX built and installed to %PHYSX_LIB_OUTPUT%