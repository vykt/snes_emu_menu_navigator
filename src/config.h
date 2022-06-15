#ifndef CONFIG_H
#define CONFIG_H

#define OPTION_SIZE 3
#define KEYMAP_SIZE 10
#define KEYMAP_DIMENSIONS 3

#define OPT_GAMEPAD 0
#define OPT_LAUNCH 1
#define OPT_ROM_PATH 2

#define ROM_MAX 23 //TODO Implement menu scrolling, then this can be raised.
#define ROM_EXTENSION_LENGTH 4

int config_populate(char * conf_path, char * options_arr, int * keymap_arr);

int config_read_roms(char * rompath, char * roms_arr, int * roms_arr_len);

#endif
