#pragma once

#include <sys/types.h>
#include <stdio.h>
#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>

#define SCREENXRES 640 // Screen width
#define SCREENYRES 480 // Screen height
#define CENTERX SCREENXRES/2 // Center of screen on x
#define CENTERY SCREENYRES/2 // Center of screen on y
#define MARGINX 16 // margins for text display
#define MARGINY 16
#define FONTSIZE 8 * 60 // Text Field Height

#define OTLEN 8

GsOT OT;
u_long ot[2][OTLEN];

char pribuff[1024];

void init_display();
void display();
