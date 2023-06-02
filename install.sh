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
