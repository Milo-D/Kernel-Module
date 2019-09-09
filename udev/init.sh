#A simple script to insert Module LDX into the
#Linux Kernel and create a Char Device in /dev/
#with the name: ldx

#Usage:
#First: Run this script as Root without any arguments.
#After that, LDX Kernel Module will be loaded into the Kernel.
#Then the script wants you to enter the Major Number of LDX.
#To find out the major number, just hit 'dmesg' in a new terminal.
#LDX has printed his major number to the logs.
#After passing the Major Number to init.sh Script, a new Char Device
#will be created: /dev/ldx

#Warning: Do not rename ldx Kernel Module (ldx.ko) before running this
#script! 

#!/bin/bash

make

echo "Loading Kernel Module..."
insmod ldx_udev.ko

echo "Done!"
lsmod | grep ldx_udev

find /dev/LDX_UDEV

echo "Finished!"