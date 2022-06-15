#ifndef MENU_H
#define MENU_H

#define TEXT_UNSELECTED 1	//white
#define TEXT_SELECTED 2		//green
#define TEXT_TITLE 3		//magenta


int menu_init(char * roms_arr, int * roms_arr_len, int * max_y, int * max_x, int * start_y, int * selection);

int menu_maintenance_check();

int menu_update(char * roms_arr, int roms_arr_len, int max_y, int max_x, int start_y, int * selection, int input, char * opt_arr, char * xorg_launch_path);

int menu_kill();

#endif
