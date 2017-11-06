KDIR := /lib/modules/$(shell uname -r)/build

obj-m += netLinkKernel.o

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
	$(CC) netLinkUser.c -o test

clean:
	rm -rf *.o *.ko *.mod.* *.cmd .module* modules* Module* .*.cmd .tmp*
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean


# 	Execute make first. This will result in a netlinkKernel.ko output among many others.
# 2. Execute $ gcc netlinkUser.c -o netlinkUser
# 3. Insert kernel module by :$ sudo insmod netlinkKernel.ko      
# 4. Run ./netlinkUser to see message and run dmesg to see debug messages
# 5. Remove module by : $ sudo rmmod netlinkKernel          
# 6. Finally make clean to remove output files.