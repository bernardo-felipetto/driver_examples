obj-m := iocontroldevice.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	$(CC) iocontrol.c -o test
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
	rm test




# all:
# 	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
# 	sudo insmod iocontrol.ko
# 	lsmod | grep iocontrol
# 	sudo mknod -m 666 /dev/iocontrol c $(major) $(minor)
# 	$(CC) -o test test.c
# 	./test
#
# clean:
# 	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
# 	sudo rmmod ioctl*
# 	# rmsf /dev/iocontrol
# 	sudo rm /dev/iocontrol
# 	rm test
