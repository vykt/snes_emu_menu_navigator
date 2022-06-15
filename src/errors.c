#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "errors.h"

#define TIME_BUF_SIZE 64

void error_handler(int err_id) {

	//TODO change to /var/log/kentaro_menu.log
	char * log_path = "/var/log/kentaro_menu.log";
	
	//TODO Array of error messages. Add new here.
	char * error_msg_array[ERROR_MESSAGE_ARRAY_SIZE] = {
		"failed to open input device for reading.",
		"failed to set device reading file descriptor as non blocking.",
		"failed to open config file for reading. Does it exist?",
		"failed to read config file values. Check formatting.",
		"configuration file incomplete.",
		"unable to read joystick input. Check connection?",
		"failed to open ROM directory. Check access privileges?",
		"too many rom images present, menu unable to display all images.",
		"failed to open /etc/xorg_launch.sh for writing. Check permissions?",
		"failed to startx, is it available?"
	};

	//Opening error log. Exiting if failed.
	FILE * log_fd;
	if ((log_fd = fopen(log_path, "a")) == NULL) {
		fprintf(stderr, "FATAL ERROR: An error has occurred. The program was unable to open the error log for writing. Exiting...\n");
		exit(-2);
	}

	//Generating error message
	time_t timestamp;
	struct tm * time_format;
	char time_buf[TIME_BUF_SIZE] = {};

	if ((time(&timestamp)) == -1) {
		strcpy(time_buf, "<no timestamp>");
	}
	
	time_format = localtime(&timestamp);
	strftime(time_buf, TIME_BUF_SIZE, "%d %b %g %H:%M:%S", time_format);

	fprintf(log_fd, "%s : Error: %s\n", time_buf, error_msg_array[err_id]);	
	fclose(log_fd);
	exit(-1);
}
