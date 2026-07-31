#!/usr/bin/env bash
set -e

echo "Installing build dependencies..."
sudo apt-get update && sudo apt-get install -y gcc g++ gperf bison flex texinfo \
    help2man make libncurses5-dev python3-dev devscripts original-awk libtool-bin automake

if [ ! -d "crosstool-ng" ]; then
    git clone https://github.com/crosstool-ng/crosstool-ng toolchain/crosstool-ng
    cd toolchain/crosstool-ng
   # git checkout tags/crosstool-ng-1.29.0
    ./bootstrap
    ./configure --enable-local
    make
    cd ..
fi

# 3. Load your pre-saved configuration and build
echo "Building the armeb-eabi toolchain..."
./crosstool-ng/ct-ng defconfig DEFCONFIG=armeb-eabi.defconfig
./crosstool-ng/ct-ng build
