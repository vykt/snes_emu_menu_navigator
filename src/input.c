#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/time.h>

#include <linux/joystick.h>

#include "input.h"
#include "config.h"
#include "errors.h"

//Testing includes
#include <stdio.h>
#include <curses.h>

int input_init(char * dev_path) {

	int fd;

	//Set descriptor to not block
	if ((fd = open(dev_path, O_RDONLY)) == -1) error_handler(INPUT_DEVICE_OPEN_ERR);
	if ((fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK)) == -1) error_handler(INPUT_DEVICE_NONBLOCK_ERR);

	return fd;
}


int input_get(int fd, int * keymap_arr, struct timeval * time_last) {

	struct timeval timestamp;
	gettimeofday(&timestamp, NULL);

	if (time_last->tv_usec + 10 > timestamp.tv_usec) {
		return INP_WAIT;
	}

	struct js_event event;
	if((read(fd, &event, sizeof(event))) == -1) {
		if (errno != 11) error_handler(INPUT_JS_CONNECT_ERR);
		return INP_NONE;
	}

	int inp_send = INP_NONE;

	//Input order:
// dpad_up, dpad_down, dpad_right, dpad_left, but_a, but_b, but_x, but_y, start, select

	int input_cmp;
	int event_arr[3] = {};

	for (int i = 0; i < KEYMAP_SIZE; ++i) {

		input_cmp = 1;

		//populate event_arr
		event_arr[0] = event.number;
		event_arr[1] = event.type;
		event_arr[2] = event.value;

		//comparison
		for (int j = 0; j < KEYMAP_DIMENSIONS; ++j) {

			//if (keymap_arr[i][j] != event_arr[j])
			if (*(keymap_arr+(i*KEYMAP_DIMENSIONS) + j) != event_arr[j]) {
				input_cmp = 0;
			}
		}

		//if input passed check, return it
		if (input_cmp) {
			refresh();
			time_last->tv_usec = timestamp.tv_usec;
			return i;
		}

	}


	return inp_send;
}


int input_close(int fd) {
	close(fd);
	return 0;
}
