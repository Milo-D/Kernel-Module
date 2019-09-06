# LDX-Kernel-Module
Simple Char Device Template

Steps to take:

You will need the Kernel Developer Package plus Kernel Headers, in order to use LDX.

1. Compile LDX by typing 'make'
2. 'chmod u+x' on all Shell Scripts
3. Read the Notes in Shell Scripts
4. Hit 'sudo ./init.sh' to load LDX Module into Kernel and to create a new Char Device
5. Navigate to 'poc' directory
6. Hit 'sudo ./build.sh' to compile the proof of concept
7. Use './ldxuser -w <data>' to write <data> to the LDX Device
8. Use './ldxuser -r' to read from the LDX Device

Note: To see some logs and/or to lookup the Major Number of LDX, use 'dmesg'.

Clean Up:

1. Hit 'sudo ./clean.sh', in order to unload the Kernel Module and to remove /dev/ldx Device
2. By running the './clean.sh' the directory will be cleaned, too

Note: This Device Template only covers open/read/write/close Operations on the Device.