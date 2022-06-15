#ifndef INPUT_H
#define INPUT_H

#include <sys/time.h>

//Input definitions
#define DPAD_UP 0
#define DPAD_DOWN 1
#define DPAD_RIGHT 2
#define DPAD_LEFT 3

#define BUT_A 4
#define BUT_B 5
#define BUT_X 6
#define BUT_Y 7

#define BUT_START 8
#define BUT_SELECT 9

//function returns
#define INP_WAIT -1
#define INP_NONE -2

int input_init(char * dev_path);
int input_get(int fd, int * keymap_arr, struct timeval * time_last);
int input_close(int fd);

#endif
