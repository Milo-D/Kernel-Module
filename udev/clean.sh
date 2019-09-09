#A simple Script to clean up the Mess, LDX
#has created. Also this Script will remove the LDX
#Kernel Module plus the char Device /dev/ldx that
#has been initialized before with init.sh

#Run this script as Root.

echo "Unloading Kernel Module LDX_UDEV..."
rmmod ldx_udev

echo "Done!"

echo "Removing /dev/LDX_UDEV"
rm /dev/LDX_UDEV

echo "Done!"

echo "Cleaning LDX_UDEV Directory..."
rm ldx_udev.ko ldx_udev.o ldx_udev.mod.o ldx_udev.mod.c modules.order Module.symvers
rm .ldx_udev.ko.cmd .ldx_udev.mod.o.cmd .ldx_udev.o.cmd
rm ./.tmp_versions/ldx_udev.mod
rmdir .tmp_versions

echo "Done!"