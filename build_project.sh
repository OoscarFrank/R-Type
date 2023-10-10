#!/usr/bin/env bash

source /etc/os-release

git submodule update --init --recursive

if [ "$ID" == "ubuntu" ]; then
    sudo apt update
    sudo apt-get install rpm
    echo "Ubuntu updated"
elif [ "$ID" == "fedora" ]; then
    sudo dnf update
    sudo sudo dnf install rpm-build
    echo "Fedora updated"
else
    echo "This is a different Linux distribution"
fi

cd server
./build.sh
cd ..

cd client
./build.sh
cd ..
