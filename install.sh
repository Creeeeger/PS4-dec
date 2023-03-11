#!/bin/bash

# Install prerequisites if root
if [ "$EUID" -ne 0 ]; then
  echo "Not root, skipping update and install"
  exit
else
  apt-get update
  apt-get -y install binutils gcc make
fi

# Delete directory if it exists and make empty directory
if [ "$PWD" != "/opt/ps4sdk" ]; then
  rm -rf /opt/ps4sdk || true
  mkdir -p /opt/ps4sdk
fi

# Build SDK
cd libPS4 || (echo "Unable to enter subdirectory" && exit)
make
cd ..

# Copy compiled SDK
if [ "$PWD" != "/opt/ps4sdk" ]; then
  cp -r libPS4 /opt/ps4sdk
  cp install.sh /opt/ps4sdk/update.sh
fi

# Clear path from .bashrc if it's there
sed -i "s/^\s*export PS4SDK=.*//gm" /etc/profile

# Add to paths
echo "export PS4SDK=/opt/ps4sdk" | tee -a /etc/profile
export PS4SDK=/opt/ps4sdk
