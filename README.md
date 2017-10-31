# Char Driver Examples

The dynamicchardriver is a simple char device, implemented using alloc_chrdev_region. BEWARE: when you list it with:

$cat /proc/devices

and run again, it isn't cleaning properly the device. Thus listing it multiple times after multiple $make commands.


The iocontrol implements a io control device with semaphores.
