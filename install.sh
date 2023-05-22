#!/bin/bash

if [[ "$OSTYPE" == "darwin"* ]]; then
    echo "Run on macOS"
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    #!/bin/bash
    # Install prerequisites if running as root
    if [ "$EUID" -eq 0 ]; then
        echo "Running as root, updating and installing prerequisites"
    else
        echo "Not root, skipping update and install"
        exit
    fi

    # Delete directory if it exists and make an empty directory
    if [ "$PWD" != "$(pwd)/ps4sdk" ]; then
        rm -rf "$(pwd)/ps4sdk" || true
        mkdir -p "$(pwd)/ps4sdk"
    fi

    # Build SDK
    cd libPS4 || (echo "Unable to enter subdirectory" && exit)
    make
    cd ..

    # Copy compiled SDK
    if [ "$PWD" != "$(pwd)/ps4sdk" ]; then
        cp -r libPS4 "$(pwd)/ps4sdk"
        cp install.sh "$(pwd)/ps4sdk/update.sh"
    fi

    # Clear path from .bash_profile if it's there
    sed -i '' '/^export PS4SDK=/d' ~/.bash_profile

    # Add to paths
    echo "export PS4SDK=$(pwd)/ps4sdk" | tee -a ~/.bash_profile
    export PS4SDK=$(pwd)/ps4sdk

elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
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
        rm -rf $(pwd)/ps4sdk || true
        mkdir -p $(pwd)/ps4sdk
    fi

    # Build SDK
    cd libPS4 || (echo "Unable to enter subdirectory" && exit)
    make
    cd ..

    # Copy compiled SDK
    if [ "$PWD" != "$(pwd)/ps4sdk" ]; then
        cp -r libPS4 $(pwd)/ps4sdk
        cp install.sh $(pwd)/ps4sdk/update.sh
    fi

    # Clear path from .bashrc if it's there
    sed -i "s/^\s*export PS4SDK=.*//gm" /etc/profile
    
    # Add to paths
    echo "export PS4SDK=$(pwd)/ps4sdk" | tee -a /etc/profile
    export PS4SDK=$(pwd)/ps4sdk
    
else
    echo "Unknown operating system"
    exit
fi
