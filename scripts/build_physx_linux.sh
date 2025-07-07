#!/bin/bash

set -e

PHYSX_BUILD_TYPE="checked"
PHYSX_ROOT="$(pwd)/external/physx"
PHYSX_LIB_OUTPUT="${PHYSX_ROOT}/lib/linux/${PHYSX_BUILD_TYPE}"

echo "[VelixFlow] Cleaning old PhysX build if exists..."
rm -rf "$PHYSX_ROOT/tmp"
mkdir -p "$PHYSX_ROOT/tmp"
cd "$PHYSX_ROOT/tmp"

echo "[VelixFlow] Cloning NVIDIA PhysX SDK..."

git clone https://github.com/NVIDIA-Omniverse/PhysX.git

xml_file="PhysX/physx/buildtools/presets/public/linux-gcc.xml"

echo [VelixFlow] Patching PhysX preset to disable GPU projects...

if grep -q 'name="PX_GENERATE_GPU_PROJECTS"' "$xml_file"; then
    sed -i -E 's@<([cC][mM][aA][kK][eE][sS][wW][iI][tT][cC][hH])[^>]*name="PX_GENERATE_GPU_PROJECTS"[^>]*value="True"@<\1 name="PX_GENERATE_GPU_PROJECTS" value="False"@g' "$xml_file"
    echo "PhysX preset patched"
fi

if grep -q 'name="PX_GENERATE_GPU_PROJECTS_ONLY"' "$xml_file"; then
    sed -i 's/\(<CMakeSwitch name="PX_GENERATE_GPU_PROJECTS_ONLY" value="\)[Tt]rue"/\1False"/' "$xml_file"
    echo "PhysX preset patched"
fi

cd PhysX/physx
echo "[VelixFlow] Generating build files..."
chmod +x generate_projects.sh
./generate_projects.sh linux-gcc
cd compiler/linux-gcc-checked/
make -j$(nproc)
echo "[VelixFlow] Moving built .a files to ${PHYSX_LIB_OUTPUT}"
mkdir -p "${PHYSX_LIB_OUTPUT}"
find ../../bin/linux.clang/${PHYSX_BUILD_TYPE} -name "*.a" -exec cp {} "${PHYSX_LIB_OUTPUT}/" \;

echo "[VelixFlow] Cleaning up PhysX source..."
cd "$PHYSX_ROOT"
rm -rf tmp

echo "[VelixFlow] PhysX built and installed to ${PHYSX_LIB_OUTPUT}"