#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <string.h>

#include <linux/limits.h>

#include "menu.h"
#include "input.h"
#include "errors.h"
#include "config.h"


//Gives starting x position to let the line be centralised.
int get_start_xpos(char * line, int max_x) {

	size_t len = strlen(line);
	if (len > max_x) {
		return -1;
	}
	else {
		return ((max_x / 2) - (len / 2));
	}
}

//Gives starting y position for displaying the entire menu. Hence called once.
int get_start_ypos(int roms_arr_len, int max_y) {

	if (roms_arr_len >= max_y - 4) {
		error_handler(MENU_ROMS_LIMIT_EXCEEDED);
	}
	else {
		return ((max_y/2) - ((roms_arr_len+2)/2));
	}
}


void fit_line(char * line, int max_x) {

	char * new_ext = "[...].sfc\0"; //len = 10
	//cut string to max_x characters and replace end with new_ext
	strcpy((line + (max_x - strlen(new_ext))), new_ext);

}


void launch_rom() {

	endwin();
	if (execl("/usr/bin/startx", "/usr/bin/startx", (char *) NULL) == -1) {
		error_handler(MENU_XORG_STARTX_ERR);
	}

}


void menu_draw(char * roms_arr, int roms_arr_len, int max_x, int start_ypos, int selected) {

	int xpos;
	int ypos = start_ypos;

	attron(COLOR_PAIR(TEXT_TITLE));

	xpos = get_start_xpos("SUPER KENTARO v1.0", max_x);
	move(ypos, xpos);
	printw("%s", "SUPER KENTARO v1.0");
	ypos += 2;
	attron(COLOR_PAIR(TEXT_UNSELECTED));

	for (int i = 0; i < roms_arr_len; i++) {
		if (i == selected) {
			attron(COLOR_PAIR(TEXT_SELECTED));
		} else if (i == selected + 1) {
			attron(COLOR_PAIR(TEXT_UNSELECTED));
		}
		
		//xpos = get_start+xpos(roms_arr[i], max_x);
		xpos = get_start_xpos((roms_arr + (i * NAME_MAX)), max_x);
		move(ypos, xpos);
		printw("%s", roms_arr + (i * NAME_MAX));
		ypos++;

	}
	refresh();
	
}


int menu_init(char * roms_arr, int * roms_arr_len, int * max_y, int * max_x, int * start_y, int * selection) {

	//Initialising curses mode.
	initscr();
	start_color();			//Init color mode; Running on R-Pi, therefore
							//colours are available, hence not checking return.
	cbreak();				//Disable line buffering.
	noecho();				//Disable input echo to screen.
	keypad(stdscr, TRUE);	//Allow for function keys to return keycodes.
	nodelay(stdscr, TRUE);	//O_NONBLOCK flag for input functions.
	//Initialised curses mode.

	//Setting menu variables & colours.
	getmaxyx(stdscr, *max_y, *max_x);
	*start_y = get_start_ypos(*roms_arr_len, *max_y);
	init_pair(TEXT_UNSELECTED, COLOR_WHITE, COLOR_BLACK);
	init_pair(TEXT_SELECTED, COLOR_GREEN, COLOR_BLACK);
	init_pair(TEXT_TITLE, COLOR_MAGENTA, COLOR_BLACK);
	*selection = 0;

	menu_draw(roms_arr, *roms_arr_len, *max_x, *start_y, *selection);

	return 0;
}


int menu_update(char * roms_arr, int roms_arr_len, int max_y, int max_x, int start_y, int * selection, int input, char * opt_arr, char * xorg_launch_path) {

	switch(input) {

		case(DPAD_UP):
			if (*selection <= 0) return 1;
			*selection = *selection - 1;
			menu_draw(roms_arr, roms_arr_len, max_x, start_y, *selection);
			break;
		case(DPAD_DOWN):
			if (*selection >= (roms_arr_len - 1)) return 1;
			*selection = *selection + 1;
			menu_draw(roms_arr, roms_arr_len, max_x, start_y, *selection);
			break;
		case(BUT_START):
			FILE * xorg_launch_fp = fopen(xorg_launch_path, "w");
			if (xorg_launch_fp == NULL) {
				error_handler(MENU_XORG_LAUNCH_WRITE_ERR);
			}
			fprintf(xorg_launch_fp, "%s %s/%s", (opt_arr+(PATH_MAX * OPT_LAUNCH)), (opt_arr+(PATH_MAX * OPT_ROM_PATH)), (roms_arr+(NAME_MAX * *selection)));
			fclose(xorg_launch_fp);
			
			//Now fork and startx
			launch_rom();			

			exit(0);
			break;
		default:
			return 1;
	}

	return 0;

}


int menu_kill() {
	endwin();

	return 0;
}
