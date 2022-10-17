#pragma once

#include <stdbool.h>

enum {
	BTN_SELECT = 1,
	BTN_L3 = 2,
	BTN_R3 = 4,
	BTN_START = 8,
	BTN_UP = 16,
	BTN_RIGHT = 32,
	BTN_DOWN = 64,
	BTN_LEFT = 128,
	BTN_L2 = 256,
	BTN_R2 = 512,
	BTN_L1 = 1024,
	BTN_R1 = 2048,
	BTN_TRIANGLE = 4096,
	BTN_CIRCLE = 8192,
	BTN_CROSS = 16384,
	BTN_SQUARE = 32768
};

typedef struct {
	unsigned char	stat;
	unsigned char	len:4;
	unsigned char	type:4;
	unsigned short	btn;
	unsigned char	rs_x,rs_y;
	unsigned char	ls_x,ls_y;
} PADTYPE;

u_char padbuff[2][34];

void init_input();
bool button_pressed(const int controller, const u_short button);
