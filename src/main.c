#include <sys/types.h>
#include <stdio.h>
#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libcd.h>
#include <malloc.h>

#include "render.h"
#include "input.h"

#define CD_SECTOR_SIZE 2048
// Bytes to sectors
#define BtoS(len) ( ( len + CD_SECTOR_SIZE - 1 ) / CD_SECTOR_SIZE )

static unsigned char heap[0x40000];

int tim_mode;           // TIM image parameters
RECT tim_prect,tim_crect;
int tim_uoffs,tim_voffs;

void init() {
	init_display();

	// Initialize CD and heap
	CdInit();
	InitHeap((u_long *)heap, sizeof(heap));

	init_input();

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

		if ( button_pressed(0, BTN_UP) )
			y--;
		if ( button_pressed(0, BTN_DOWN) )
			y++;
		if ( button_pressed(0, BTN_LEFT) )
			x--;
		if ( button_pressed(0, BTN_RIGHT) )
			x++;

		GsBOXF tile = {
			0,
			x, y,
			64, 64,
			255, 255, 0
		};
		GsSortBoxFill(&tile, &OT, 0);

		GsBOXF tile2 = {
			0,
			200, 200,
			128, 64,
			255, 0, 255
		};
		GsSortBoxFill(&tile2, &OT, 0);

		GsSetOrign(-x+CENTERX, -y+CENTERY);

		// x = (x+1) % SCREENXRES;
		// y = (y+1) % SCREENYRES;

		display();

	}

	return 0;

}
