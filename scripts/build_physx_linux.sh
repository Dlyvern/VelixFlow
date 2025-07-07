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