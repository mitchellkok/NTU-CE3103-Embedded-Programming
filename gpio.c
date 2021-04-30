#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/ioctl.h>
#include <linux/gpio.h>

/* you may want to look at the contents of this file, to understand
the various elements of the data structure shown in the code below */


int main(int argc, char *argv[ ])
{
	pthread_t thread_led, thread_b11;

	int fd0 = open("/dev/gpiochip0",O_RDWR); // open the file descriptor

	struct gpiochip_info cinfo;
	ioctl(fd0,GPIO_GET_CHIPINFO_IOCTL,&cinfo); // get the chip information
	fprintf(stdout, "GPIO chip 0: %s, \"%s\", %u lines\n", cinfo.name, cinfo.label, cinfo.lines);

	//////// LEDS ////////
	struct gpiohandle_request req_GYR; // Green and Yellow
	struct gpiohandle_data data_GYR; // for data bit

	req_GYR.lines = 3; // 3 pins in this handler (3 lines requested)
	req_GYR.lineoffsets[0] = 4; //pin 4 - Green LED (assigning line 0)
	req_GYR.lineoffsets[1] = 17; // pin 17 - Yellow LED (assigning line 1)
	req_GYR.lineoffsets[2] = 27; // pin 27 - Red LED

	req_GYR.flags = GPIOHANDLE_REQUEST_OUTPUT; // set handle to be output
	data_GYR.values[0] = 1;  // set initial value of Green LED to High (ON)
	data_GYR.values[1] = 0;  // set initial value of Yellow LED to Low (OFF)
	data_GYR.values[2] = 0;  // set initial value of Red LED to Low (OFF)
	ioctl(fd0, GPIO_GET_LINEHANDLE_IOCTL, &req_GYR); // now get the line handler req_GYR

	//////// BUTTON ////////
	struct gpiohandle_request req_pb; // for push button
	struct gpiohandle_data data_pb; // for data bit

	req_pb.lines = 1;
	req_pb.lineoffsets[0] = 11; //pin 11 push button
	req_pb.flags = GPIOHANDLE_REQUEST_INPUT;
	ioctl(fd0, GPIO_GET_LINEHANDLE_IOCTL, &req_pb);

	printf("starting\n");

	//////// MAIN ROUTINE ////////
	int cnt = 0, pressed = 0; 
	for(;;) { // int i = 0; i < 5; ++i){
		ioctl(req_GYR.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data_GYR); // output data bits
		usleep(1000); // sample every 1ms

		if (cnt == 1000) { 	// toggle every 1s
			if (pressed == 1) {
				data_GYR.values[0] = !data_GYR.values[0]; 	// toggle green
				data_GYR.values[1] = !data_GYR.values[1];	// toggle yellow
				data_GYR.values[2] = !data_GYR.values[2];
			}
			else {
				data_GYR.values[0] = !data_GYR.values[0];
                                data_GYR.values[1] = !data_GYR.values[1];
				data_GYR.values[2] = 0;
			}
			cnt = 0;

		}
		cnt++;

		ioctl(req_pb.fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data_pb); //input bit at data_pb.values[0];
		if (data_pb.values[0] == 1) {
			pressed = 1;
			//data_GYR.values[2] = 1;	// set red
		}
		else {
			pressed = 0;
			//data_GYR.values[2] = 0;	// set red
		}
	}

	close(req_GYR.fd); // release line
	close(req_pb.fd); // release line
	close(fd0); // close the file

	printf("Exiting program\n");
	exit(EXIT_SUCCESS);
}//main
