#ifndef IOCTLDEVICE_H
#define IOCTLDEVICE_H

#include <linux/ioctl.h>

#define DEVICE_NAME "iocontroldevice"	// Name of device
#define CLASS_NAME "iocontroldevice"	// Class name
#define MAJOR 21			// The MAJOR device number
#define MINOR 6				// The MINOR device number
#define COUNT 0				// the number of consecutive device numbers required
#define MINORCOUNT 1				// the number of consecutive minor numbers corresponding to this device

#define SEMAPHORE_HOLD    _IOR(MAJOR, 0, int *)	//
#define READ_BUFFER       _IOWR(MAJOR, 1, int *)	//
#define WRITE_BUFFER      _IOR(MAJOR, 2, int *)	//
#define SEMAPHORE_RELEASE _IOR(MAJOR, 3, int *)
// #define IOCTL_GET_VAR 	_IOWR(MAJOR, 1, int *)	//
//
// #define IOCTL_DOWN_VAR 	_IOR(MAJOR, 0, int *)	//
// #define IOCTL_UP_VAR 	_IOR(MAJOR, 1, int *)	//
// #define IOCTL_GET_VAR 	_IOWR(MAJOR, 2, int *)	//

#endif //DEVICE_h
