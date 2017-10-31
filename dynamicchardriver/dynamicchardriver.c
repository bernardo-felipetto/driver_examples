#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h> // required for various structures related to files liked fops.
#include <asm/uaccess.h> // required for copy_from and copy_to user functions
#include <linux/semaphore.h>
#include <linux/cdev.h>

MODULE_LICENSE("GPL");			// The license type -- this affects available functionality
MODULE_AUTHOR("Bernardo Felipetto");   


#define DEVICE_NAME "dynamicchardriver"		//Name of your device that should be associated with the major numbers. The same name will appear in /proc/devices.
#define CLASS_NAME "dDev"		// The device class -- this is a character device driver
#define MAJOR 250				// Number for the Major
#define MINOR 7
#define COUNT 1 //The number of contiguous set of major numbers that you want to be allocated.
#define DEVICENUMBER 1 //The number of number of device numbers that are associated with this cdev structure.

dev_t dev;
dev_t dev_no;

int ret;
static int times_opened = 0;

struct cdev *kernel_cdev;

struct device{
  char array[100];
  struct semaphore sem;
}char_arr;



static int 		dev_open	(struct inode *, struct file *);
static int 		dev_release	(struct inode *, struct file *);
static ssize_t 	dev_read	(struct file *, char *, size_t, loff_t *);
static ssize_t	dev_write	(struct file *, const char *, size_t, loff_t *);


static struct file_operations fops =
{
  .owner		= THIS_MODULE,
  .open 		= dev_open,
  .read 		= dev_read,
  .write 		= dev_write,
  .release 	= dev_release,
};

/**
*
*
*INIT FUNCTION
*
*
*/

static int  char_arr_init(void){
  printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);

  kernel_cdev = cdev_alloc();
  kernel_cdev->ops = &fops;
  kernel_cdev->owner = THIS_MODULE;

  ret = alloc_chrdev_region(&dev_no, 0, COUNT, DEVICE_NAME);
  if (ret < 0){
    printk(KERN_ALERT "Error to register device!\n");
    return ret;
  }

  dev = MKDEV(MAJOR, MINOR);
  sema_init(&char_arr.sem,1);
  printk (" The major number for your device is %d\n", MAJOR);
  ret = cdev_add(kernel_cdev, dev, DEVICENUMBER);
  if (ret < 0){
    printk(KERN_ALERT "Error! Unable to add CDEV to Kernel\n");
    return ret;
  }

  printk(KERN_INFO "Nice! Device Created!");

  return 0;
}


/**
*
*
*EXIT FUNCTION
*
*
*/

static void char_arr_cleanup(void){
  dev_t major = MKDEV(MAJOR, MINOR);
  printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
  printk(KERN_ALERT "MAJOR IS %d @@@@@@@@@@@@@@@@@@@@@@@@\n", major);
  cdev_del(kernel_cdev);
  unregister_chrdev_region(major, DEVICENUMBER);
}


/**
*
*
*OPEN CLOSE READ WRITE FUNCTION
*
*
*/

static int dev_open(struct inode *inode, struct file *file){
  printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
  times_opened++;
  printk(KERN_ALERT "Device opened %d times\n", times_opened);
  if(down_interruptible(&char_arr.sem)) {
    printk(KERN_INFO " could not hold semaphore");
    return -1;
  }
  return 0;  //sucess
}

static ssize_t dev_read(struct file *file, char *buffer, size_t length, loff_t *offset){
  printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
  unsigned long ret;

  ret = copy_to_user(buffer, char_arr.array, length);

  return ret;
}

static ssize_t dev_write(struct file *file, const char *buffer, size_t length, loff_t *offset){
  printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
  unsigned long ret;

  ret = copy_from_user(char_arr.array, buffer, length);

  return length; //number of bytes in the buffer
}

static int dev_release(struct inode *inode, struct file *file){
  printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);

  up(&char_arr.sem);

  printk(KERN_INFO "Device successfully closed.\n");
  return 0; //sucess
}



module_init(char_arr_init);
module_exit(char_arr_cleanup);
