#!/bin/bash

echo "Run on Linux"
# Install prerequisites if root
if [ "$EUID" -ne 0 ]; then
  echo "Not root, skipping update and install"
  exit
else
    apt-get update
    apt-get -y install binutils gcc make
fi

# Delete directory if it exists and make empty directory
if [ "$PWD" != "$(pwd)/ps4sdk" ]; then
    rm -rf "$(pwd)/ps4sdk" || true
    mkdir -p "$(pwd)/ps4sdk"
fi

# Build SDK
cd libPS4 || (echo "Unable to enter subdirectory" && exit)
make
cd ..
