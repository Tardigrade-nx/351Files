#!/bin/env bash

TOOLCHAIN_DIR="$HOME"/Dev/351ELEC/build.351ELEC-RG351V.aarch64/toolchain
VERSION="$1"

set -eE

# $1: DEVICE
# $2: FIRMWARE
# $3: START_PATH
# $4: RES_PATH
build351Files() {
   DEVICE="$1"
   FIRMWARE="$2"
   START_PATH="$3"
   RES_PATH="$4"
   make clean
   make CC="$TOOLCHAIN_DIR"/bin/aarch64-libreelec-linux-gnueabi-g++ DEVICE="$DEVICE" SDL2_CONFIG="$TOOLCHAIN_DIR"/aarch64-libreelec-linux-gnueabi/sysroot/usr/bin/sdl2-config START_PATH="$START_PATH" RES_PATH="$RES_PATH"
   mkdir -p build/351Files
   cp -r 351Files README.md res build/351Files
   cp launchers/"$FIRMWARE"/351Files.sh build
   cd build
   tar zcf 351Files-"$VERSION"_"$DEVICE"_"$FIRMWARE".tgz 351Files.sh 351Files
   rm -rf 351Files.sh 351Files
   cd ..
}

# Clean up previous builds
rm -rf build

# Build for 351V, 351ELEC
build351Files RG351V 351ELEC /storage/roms ./res

# Build for 351P, 351ELEC
build351Files RG351P 351ELEC /storage/roms ./res

# Build for 351V, ArkOS
build351Files RG351V ArkOS /roms ./res

# Build for 351P, ArkOS
build351Files RG351P ArkOS /roms ./res
