obj-m := dynamicchardriver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	sudo insmod dynamicchardriver.ko
	lsmod | grep dynamicchardriver
	sudo mknod -m 666 /dev/dynamicchardriver c $(major) $(minor)
	$(CC) -o test test.c
	./test

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	sudo rmmod dynamicchardriver
	# sudo rmmod ioctl*
	# sudo rm /dev/ioctl*
	sudo rm /dev/dynamicchardriver
	rm test
