#!/bin/bash
make clean
bash install.sh
chmod +rwx Makefile
export PS4SDK=/opt/ps4sdk
make
chmod +777 ps4_decryptor.bin
