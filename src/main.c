#include <sys/types.h>
#include <stdio.h>
#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libcd.h>
#include <malloc.h>

#include "render.h"

#define CD_SECTOR_SIZE 2048
// Bytes to sectors
#define BtoS(len) ( ( len + CD_SECTOR_SIZE - 1 ) / CD_SECTOR_SIZE )


static unsigned char heap[0x40000];


u_char padbuff[2][34];

int tim_mode;           // TIM image parameters
RECT tim_prect,tim_crect;
int tim_uoffs,tim_voffs;

// Pad stuff (omit when using PSn00bSDK)
#define PAD_SELECT      1
#define PAD_L3          2
#define PAD_R3          4
#define PAD_START       8
#define PAD_UP          16
#define PAD_RIGHT       32
#define PAD_DOWN        64
#define PAD_LEFT        128
#define PAD_L2          256
#define PAD_R2          512
#define PAD_L1          1024
#define PAD_R1          2048
#define PAD_TRIANGLE    4096
#define PAD_CIRCLE      8192
#define PAD_CROSS       16384
#define PAD_SQUARE      32768

typedef struct _PADTYPE
{
    unsigned char	stat;
    unsigned char	len:4;
    unsigned char	type:4;
    unsigned short	btn;
    unsigned char	rs_x,rs_y;
    unsigned char	ls_x,ls_y;
} PADTYPE;

void init() {
	init_display();

	// Initialize CD and heap
	CdInit();
	InitHeap((u_long *)heap, sizeof(heap));

	// Initialize the pads
	InitPAD( padbuff[0], 34, padbuff[1], 34 );

	// Begin polling
	StartPAD();

}

u_long* load_file(char *filename) {
	// Get file position from filename
	CdlFILE file_pos = {0};
	CdSearchFile( &file_pos, filename);

	// Allocate memory
	u_long *buffer = malloc( BtoS(file_pos.size) * CD_SECTOR_SIZE );

	CdControl(CdlSetloc, (u_char *)&file_pos.pos, 0);
	CdRead( (int)BtoS(file_pos.size), (u_long *)buffer, CdlModeSpeed );
	CdReadSync(0, 0);

	return buffer;

}

int main() {
	init();
	short x = 32, y =32;

	char* text_file = (char*)load_file("\\ANNABEL.TXT;1");

	GsClearOt(0, 0, &OT);
	GsSortClear(64, 64, 128, &OT);

	while (1) {
		// FntLoad(960, 0);
		// FntOpen(MARGINX+x, MARGINY+y, SCREENXRES - MARGINX * 2, FONTSIZE, 0, 1024 );
		// FntPrint("%s", text_file);
		// FntFlush(-1);

		// Parse controller input
		PADTYPE* pad = (PADTYPE*)padbuff[0];

		// Only parse inputs when a controller is connected
		if( pad->stat == 0 )
		{
			// Only parse when a digital pad,
			// dual-analog and dual-shock is connected
			if( ( pad->type == 0x4 ) ||
				( pad->type == 0x5 ) ||
				( pad->type == 0x7 ) )
			{
				if( !(pad->btn&PAD_UP) )            // test UP
				{
					y--;
				}
				else if( !(pad->btn&PAD_DOWN) )       // test DOWN
				{
					y++;
				}
				if( !(pad->btn&PAD_LEFT) )          // test LEFT
				{
					x--;
				}
				else if( !(pad->btn&PAD_RIGHT) )    // test RIGHT
				{
					x++;
				}
			}
		}

		GsBOXF tile = {
			0,
			x, y,
			64, 64,
			255, 255, 0
		};
		GsSortBoxFill(&tile, &OT, 0);

		// x = (x+1) % SCREENXRES;
		// y = (y+1) % SCREENYRES;

		display();

	}

	return 0;

}
