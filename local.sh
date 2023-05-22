#!/bin/bash
make clean
bash install.sh
chmod +rwx Makefile
export PS4SDK=$(pwd)/ps4sdk
make
chmod +777 PS4_ubu.bin
chmod +777 PS4_mac.bin
chmod +777 PS4.exe
