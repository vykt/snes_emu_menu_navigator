#ifndef ERRORS_H
#define ERRORS_H


#define ERROR_MESSAGE_ARRAY_SIZE 10

//Error code definitions:
#define INPUT_DEVICE_OPEN_ERR 0
#define INPUT_DEVICE_NONBLOCK_ERR 1
#define CONFIG_READ_ERR 2
#define CONFIG_FORMAT_ERR 3
#define CONFIG_INCOMPLETE_ERR 4
#define INPUT_JS_CONNECT_ERR 5
#define CONFIG_ROMS_READ_ERR 6
#define MENU_ROMS_LIMIT_EXCEEDED 7
#define MENU_XORG_LAUNCH_WRITE_ERR 8
#define MENU_XORG_STARTX_ERR 9

void error_handler(int err_id);


#endif
