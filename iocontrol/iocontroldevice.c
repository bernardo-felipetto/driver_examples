#include <linux/device.h>		// Header to support the kernel Driver Model
#include <linux/init.h>			// Macros used to mark up functions e.g. __init __exit
#include <linux/kernel.h>		// Contains types, macros, functions for the kernel
#include <linux/module.h>		// Core header for loading LKMs into the kernel
#include <linux/fs.h>			// Header for the Linux file system support
#include <asm/uaccess.h>		// Required for the copy to user function
#include <linux/cdev.h>
#include <linux/time.h>
#include <linux/semaphore.h>
#include "iocontroldevice.h"

MODULE_LICENSE("GPL");			// The license type -- this affects available functionality
MODULE_AUTHOR("Bernardo Felipeto");	// The author -- visible when you use modinfo

static int buffer;
static int times_opened = 0;
static unsigned int device_number = 0;

struct cdev *kernel_cdev;

int ret;
dev_t dev_num;

static struct class* dev_char_class = NULL;
static struct device* dev_char_device = NULL;


static int 		dev_open	(struct inode *, struct file *);
static int 		dev_release	(struct inode *, struct file *);
long dev_ioctl (struct file *, unsigned int, int *);


static struct file_operations fops =
{
  .owner		= THIS_MODULE,
  .open 		= dev_open,
  .release 	= dev_release,
  .unlocked_ioctl	= dev_ioctl,
};

int number = 0;
struct semaphore sem;
// struct iodevice {
// }result;

/**
*
*
*INIT FUNCTION
*
*
*/

static int __init device_init(void){

  printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);

  dev_num = MKDEV(MAJOR, MINOR);

  ret = register_chrdev_region(dev_num, COUNT, DEVICE_NAME);


  if (ret < 0){
    printk(KERN_ALERT "Error to register device!\n");
    return ret;
  }

  kernel_cdev = cdev_alloc();
  kernel_cdev->ops = &fops;
  kernel_cdev->owner = THIS_MODULE;

  printk (" The major number for your device is %d\n", MAJOR);

  sema_init(&sem, 1);

  device_number = device_number + 1;
  ret = cdev_add(kernel_cdev, dev_num, device_number);

  if (ret < 0){
    printk(KERN_ALERT "Error! Unable to add CDEV to Kernel\n");
    return ret;
  }

  dev_char_class = class_create(THIS_MODULE, CLASS_NAME);
  if (IS_ERR(dev_char_class)){

    unregister_chrdev(MAJOR, DEVICE_NAME);
    printk(KERN_ALERT "Failed to register device class!\n");
    return PTR_ERR(dev_char_class);

  }

  dev_char_device = device_create(dev_char_class, NULL, dev_num, NULL, DEVICE_NAME );
  if(IS_ERR( dev_char_device )){

    class_destroy( dev_char_class );
    unregister_chrdev( MAJOR, DEVICE_NAME );
    printk(KERN_ALERT "Failed to create device\n");
    return PTR_ERR(dev_char_device);

  }

  printk(KERN_INFO "!!! Device Created !!!");

  return 0;
}


/**
*
*
*EXIT FUNCTION
*
*
*/

static void __exit device_exit(void){
  printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
  dev_num = MKDEV(MAJOR, MINOR);
  device_destroy(dev_char_class, dev_num);
  class_unregister(dev_char_class);
  class_destroy(dev_char_class);
  cdev_del(kernel_cdev);
  unregister_chrdev_region(dev_num, device_number);
  device_number = device_number - 1;
}


/**
*
*
*OPEN CLOSE IOCTL
*
*
*/

static int dev_open(struct inode *inode, struct file *file){
  printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);

  // if(down_interruptible(&result.sem)) {
  //   printk(KERN_INFO " could not hold semaphore");
  //   return -1;
  // }

  times_opened++;

  printk(KERN_ALERT "Device opened %d times\n", times_opened);

  return 0;  //sucess
}

static int dev_release(struct inode *inode, struct file *file){
  printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
  // up(&result.sem);
  printk(KERN_INFO "Device successfully closed.\n");
  return 0; //sucess
}

long dev_ioctl(struct file *file, unsigned int ioctl_num, int *ioctl_param){

  //int *temp = (int *)(ioctl_param);

  switch(ioctl_num){

    case SEMAPHORE_HOLD:
      down_interruptible(&sem);
      // copy_from_user(&buffer, temp, sizeof(int));
      // // result.res += buffer;
    break;

    case READ_BUFFER:
      copy_to_user(ioctl_param, &number, sizeof(number));
    break;

    case WRITE_BUFFER:
      copy_from_user(&number, ioctl_param, sizeof(ioctl_param));
    break;

    case SEMAPHORE_RELEASE:
      up(&sem);
    break;
    return 0;
  }
}

module_init(device_init);
module_exit(device_exit);
