#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "iocontroldevice.h"

#define DEVICE_FILE_ORIGIN "/dev/iocontroldevice"

void semaphore_hold(int file_desc){
	ioctl(file_desc, SEMAPHORE_HOLD, NULL);
}

void read_buffer(int file_desc, int *value){
	ioctl(file_desc, READ_BUFFER, value);
}

void write_buffer(int file_desc, int *value){
	ioctl(file_desc, WRITE_BUFFER, value);
}

void semaphore_release(int file_desc){
	ioctl(file_desc, SEMAPHORE_RELEASE, NULL);
}





// void write_buffer(int file_desc, int *value){
// 	int ret;
// 	ret = ioctl(file_desc, READ_BUFFER, value);
// 	if (ret < 0){
// 		printf("Error: ioctl_up_var [%d]\n", ret);
// 		exit(-1);
// 	}
// }
//
//
//
// int ioctl_get_var(int file_desc, int *value){
// 	return ioctl(file_desc, IOCTL_GET_VAR, value);
// }

int main(){
	int file_desc;
	int number;

	file_desc = open(DEVICE_FILE_ORIGIN, 0);
	if (file_desc < 0){
		printf("Can't open device file: %s\n", DEVICE_FILE_ORIGIN);
		exit(-1);
	}

	int i;
	for (i = 0; i < 1000000; i++) {

		//atualizar number, pegando o valor do Driver
		//.
		//incrementar number e devolver o valor atualizado para o driver
		//.
		semaphore_hold(file_desc);
		read_buffer(file_desc, &number);
		number++;
		printf("Value: %d\n", number);
		write_buffer(file_desc, &number);
		semaphore_release(file_desc);
	}

	printf("IOCTL: BYE! =D\n");

	close(file_desc);
}
