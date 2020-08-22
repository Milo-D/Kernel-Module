# Kernel-Module
Simple Char Device Template

This Repository demonstrates reading/writing operations on a Char Device. It contains two different
Kernel Modules, which do the same thing.

1. LDX: Registration using Major/Minor Numbers (found in major directory)
2. LDX_UDEV: Same Module but using udev (found in udev directory)

In each directory (major and udev) you will find a C-Program (poc/ldxuser) that simulates a Driver-Instance,
to perform simple Reading/Writing Operations.

In each directory (major and udev) you will find two Shell-Scripts (init.sh and clean.sh).
(i): init.sh compiles the Kernel Module (via make) and inserts the compiled Module to the Kernel.
     (if you use the major version, specify the Major Number after running init.sh)
(ii): clean.sh removes all files that are created by init.sh and removes the Module from the Kernel.
      The Char Device in /dev/ will be removed, too.

Steps to take:

You will need the Kernel Developer Package plus Kernel Headers, in order to use LDX.

First, pick a Version (major or udev), then:

1. 'chmod u+x' on all Shell Scripts
2. Read the Notes in Shell Scripts
3. Hit 'sudo ./init.sh' to load LDX/LDX_UDEV Module into Kernel and to create a new Char Device
4. Navigate to 'poc' directory
5. Hit 'sudo ./build.sh' to compile the proof of concept
6. Use './ldxuser -w <data>' to write <data> to the LDX/LDX_UDEV Device
7. Use './ldxuser -r' to read from the LDX/LDX_UDEV Device

Note: To see some logs and/or to lookup the Major Number of LDX, use 'dmesg'.

Clean Up:

1. Hit 'sudo ./clean.sh', in order to unload the Kernel Module and to remove /dev/ldx Device
2. By running the './clean.sh' the directory will be cleaned, too

Note: This Device Template only covers open/read/write/close Operations on the Device.
