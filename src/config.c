#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#include <linux/limits.h>

#include "config.h"
#include "errors.h"


int int_pow(int op, int exp) {
	
	int ret = 1;
	while (exp > 0) {
		ret *= op;
		--exp;
	}
	
	return ret;
}


void config_populate_options(char * options_arr, int options_arr_offset) {

	char * line_segment;
	if ((line_segment = strtok(NULL, "="))  == NULL) {
		error_handler(CONFIG_FORMAT_ERR);
	}
	if (line_segment[strlen(line_segment)-1] == '\n')
		line_segment[strlen(line_segment)-1] = '\0';

	strcpy(options_arr+(options_arr_offset*PATH_MAX), line_segment);
}


void config_populate_keymap(int * keymap_arr, int keymap_arr_offset) {

	char * line_segment;
	char * value_segment;
	if ((line_segment = strtok(NULL, "=")) == NULL) {
		error_handler(CONFIG_FORMAT_ERR);
	}
	if (line_segment[strlen(line_segment)-1] == '\n')
		line_segment[strlen(line_segment)-1] = '\0';

	//Split value into 3 properties
	for (int i = 0; i < 3; ++i) {
		//Splitting - first iteration
		if(i == 0) {
			if ((value_segment = strtok(line_segment, ",")) == NULL) {
				error_handler(CONFIG_FORMAT_ERR);
			}
		//Splitting - second & third iterations
		} else {
			if ((value_segment = strtok(NULL, ",")) == NULL) {
				error_handler(CONFIG_FORMAT_ERR);
			}
		}
		//keymap_arr[keymap_arr_offset][i] = (int) value_segment
		*(keymap_arr+(keymap_arr_offset*KEYMAP_DIMENSIONS)+i) = atoi(value_segment);
	

	}
}


int config_populate(char * conf_path, char * options_arr, int * keymap_arr) {


	//TODO options defined here, change here if needed.
	char * options_match[OPTION_SIZE] = {
		"gamepad",
		"launch",
		"rom_path"
	};

	//TODO keymap defined here.
	char * keymap_match[KEYMAP_SIZE] = {
		"dpad_up",
		"dpad_down",
		"dpad_right",
		"dpad_left",

		"but_a",
		"but_b",
		"but_x",
		"but_y",

		"start",
		"select"
	};

	char * conf_line = malloc(PATH_MAX * sizeof(char));
	char * conf_line_segment;
	size_t conf_line_size = 0;
	/*	conf_completion equals 2^0 + 2^1 + ... + 2^<num_elements>, where
	 *	<num_elements> is the number of expected elements in the config
	 *	including both options and keymap. Think of chmod, with each
	 *	element found, that element's number (in form 2^n) is subtracted
	 *	from conf_completion. When config is finished reading,
	 *	conf_completion will equal 0 if all needed elements were read.
	 *	
	 *	The current total for all elements is 8191, hence the line below.
	 */
	int conf_completion = 8191;
	FILE * fd;
	if ((fd = fopen(conf_path, "r")) == NULL) error_handler(CONFIG_READ_ERR);

	//Reading config file
	while ((getline(&conf_line, &conf_line_size, fd)) > 0) {

		//Getting identifier value from string.
		if(conf_line[0] == '#' || conf_line[0] == '\n') continue;
		conf_line_segment = strtok(conf_line, "=");

		//Checking identifier against options_arr
		for (int i = 0; i < OPTION_SIZE; ++i) {
			if (strcmp(options_match[i], conf_line_segment) == 0) {		
				//Assigning values from config to memory
				config_populate_options(options_arr, i);
				conf_completion -= int_pow(2, i);
			}
		}
		
		//Checking identifier against keymap_arr
		for (int i = 0; i < KEYMAP_SIZE; ++i) {
			if (strcmp(keymap_match[i], conf_line_segment) == 0) {
				//Assigning values from config to memory
				config_populate_keymap(keymap_arr, i);
				conf_completion -= int_pow(2, OPTION_SIZE+i);
			}
		}

	}
	free(conf_line);
	fclose(fd);

	if (conf_completion != 0) {
		error_handler(CONFIG_INCOMPLETE_ERR);
	}

	return 0;
}


int config_read_roms(char * rompath, char * roms_arr, int * roms_arr_len) {

	struct dirent * dir; //dir->d_name (char *)
	char * name_buffer = malloc(NAME_MAX * sizeof(char));
	int len_buffer;
	DIR *ds;

	ds = opendir(rompath);
	if (ds == NULL) {
		error_handler(CONFIG_ROMS_READ_ERR);
	};

	*roms_arr_len = 0;

	while ((dir = readdir(ds)) != NULL) {
		if (strlen(dir->d_name) <= 4) continue; //smallest possible name is X.sfc (5)
		
		//Checking for the .sfc extension
		len_buffer = strlen(dir->d_name);
		//name_buffer = last 4 characters of dir->d_name.
		strcpy(name_buffer, (dir->d_name+(len_buffer - ROM_EXTENSION_LENGTH)));
		
		if (!(strcmp(name_buffer, ".sfc"))) {
			strcpy((roms_arr+(NAME_MAX * *roms_arr_len)), dir->d_name);
			*roms_arr_len = *roms_arr_len + 1;
		}

	}

	free(name_buffer);
	return 0;

}
