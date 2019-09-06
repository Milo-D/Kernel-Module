#A simple Script to clean up the Mess, LDX
#has created. Also this Script will remove the LDX
#Kernel Module plus the char Device /dev/ldx that
#has been initialized before with init.sh

#Run this script as Root.

echo "Unloading Kernel Module LDX..."
rmmod ldx

echo "Done!"

echo "Removing /dev/ldx"
rm /dev/ldx

echo "Done!"

echo "Cleaning LDX Directory..."
rm ldx.ko ldx.o ldx.mod.o ldx.mod.c modules.order Module.symvers
