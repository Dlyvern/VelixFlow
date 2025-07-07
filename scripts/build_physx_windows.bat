@echo off
setlocal enabledelayedexpansion

set PHYSX_BUILD_TYPE=checked
set PHYSX_ROOT=%cd%\external\physx
set PHYSX_TMP=%PHYSX_ROOT%\tmp
set PHYSX_LIB_OUTPUT=%PHYSX_ROOT%\lib\windows\%PHYSX_BUILD_TYPE%

echo [VelixFlow] Cleaning old PhysX build if exists...
rmdir /S /Q "%PHYSX_TMP%" 2>nul
mkdir "%PHYSX_TMP%"
cd "%PHYSX_TMP%"

echo [VelixFlow] Cloning NVIDIA PhysX SDK...
git clone https://github.com/NVIDIA-Omniverse/PhysX.git

cd PhysX\physx

echo [VelixFlow] Generating Visual Studio project files...
generate_projects.bat vc17win64

echo [VelixFlow] Building PhysX in %PHYSX_BUILD_TYPE% mode...
msbuild compiler\vc17win64\PhysX.sln /p:Configuration=%PHYSX_BUILD_TYPE% /p:Platform=x64 /m

echo [VelixFlow] Moving built .lib files to %PHYSX_LIB_OUTPUT%...
mkdir "%PHYSX_LIB_OUTPUT%"
for /r ".\bin\win.x86_64.vc143.%PHYSX_BUILD_TYPE%" %%f in (*.lib) do (
    copy "%%f" "%PHYSX_LIB_OUTPUT%" >nul
)

echo [VelixFlow] Cleaning up PhysX source...
cd "%PHYSX_ROOT%"
rmdir /S /Q "tmp"

echo [VelixFlow] ✅ PhysX built and installed to %PHYSX_LIB_OUTPUT%
