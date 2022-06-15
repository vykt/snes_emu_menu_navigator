#include <stdlib.h>

#include <linux/joystick.h>
#include <linux/limits.h>

#include "menu.h"
#include "config.h"
#include "input.h"

//testing includes
#include <unistd.h> //sleep()
#include <stdio.h>
#include <sys/time.h>

int main() {

	char * conf_path = "/etc/kentaro/kentaro.conf";
	char * xorg_launch_path = "/etc/kentaro/xorg_launch.sh";

	//Input variables
	struct timeval inp_time_last;
	inp_time_last.tv_sec = 0;
	inp_time_last.tv_usec = 0;
	int received_input = INP_NONE;

	//Config read
	char * opt_arr = malloc((OPTION_SIZE * PATH_MAX) * sizeof(char));
	int * keymap_arr = malloc((KEYMAP_SIZE * KEYMAP_DIMENSIONS) * sizeof(int));
	char * roms_arr = malloc((ROM_MAX * NAME_MAX) * sizeof(char));
	int * roms_arr_len = malloc(sizeof(int));

	config_populate(conf_path, opt_arr, keymap_arr);
	config_read_roms((opt_arr+(PATH_MAX*OPT_ROM_PATH)), roms_arr, roms_arr_len);

	//Menu variables
	int max_y = 0;
	int max_x = 0;
	int start_y = 0;
	int selected_game = 0;
	int * max_y_ptr = &max_y;
	int * max_x_ptr = &max_x;
	int * start_y_ptr = &start_y;
	int * selected_game_ptr = &selected_game;

	//Initialise input
	int fd = input_init(opt_arr);

	//Initialise screen
	menu_init(roms_arr, roms_arr_len, max_y_ptr, max_x_ptr, start_y_ptr, selected_game_ptr);

	//TODO debug, remove
	//FILE * fd = fopen(xorg_launch_path)
	//printf("'%s %s'\n", (opt_arr+(PATH_MAX * OPT_LAUNCH)), (roms_arr+(NAME_MAX*1)));


	//MAIN LOOP
	while (1) {

		received_input = input_get(fd, keymap_arr, &inp_time_last);
		if (received_input < 0) {
			//Last input too soon or no input available.
			continue;
		} else {
			//Input received.
			menu_update(roms_arr, *roms_arr_len, *max_y_ptr, *max_x_ptr, *start_y_ptr, selected_game_ptr, received_input, opt_arr, xorg_launch_path);
		}
	}
	//END MAIN LOOP
	


	//Shutting down
	input_close(fd);
	menu_kill();

	printf("SHUTTING DOWN: input closed, menu killed...\n"); //TODO debug, remove

	//Free memory, end test
	free(opt_arr);
	free(keymap_arr);
	free(roms_arr);
	free(roms_arr_len);

	return 0;
}
