#include <sys/types.h>
#include <stdbool.h>
#include <stdio.h>
#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libcd.h>
#include <malloc.h>

#include "render.h"
#include "input.h"
#include "objects.h"
#include "vector.h"

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

	Object player = {
		OBJECT_PLAYER,
		0,
		(vec2){32, 32},
		(vec2){0, 0},
		0, 127, 192,
		32, 48,
		true, true
	};

	Object floor = {
		OBJECT_FLOOR,
		0,
		(vec2){200, 200},
		(vec2){0, 0},
		64, 64, 64,
		128, 64,
		true, true
	};

	while (1) {
		// FntLoad(960, 0);
		// FntOpen(MARGINX+x, MARGINY+y, SCREENXRES - MARGINX * 2, FONTSIZE, 0, 1024 );
		// FntPrint("%s", text_file);
		// FntFlush(-1);

		if ( button_pressed(0, BTN_UP) )
			y-=2;
		if ( button_pressed(0, BTN_DOWN) )
			y+=2;
		if ( button_pressed(0, BTN_LEFT) )
			x-=2;
		if ( button_pressed(0, BTN_RIGHT) )
			x+=2;

		player.position = (vec2){x, y};

		render_object(&player);
		render_object(&floor);

		GsSetOrign(-x+CENTERX, -y+CENTERY);

		display();

	}

	return 0;

}
