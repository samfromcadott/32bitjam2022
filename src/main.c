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
#include "util.h"
#include "player.h"

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

	Player player = {
		0,
		(vec2){32, 32},
		(vec2){0, 0},
		20,
		5, 1,
		5, 1,
		2, 2,
		0, 127, 192,
		32, 48,
		true, false, false
	};

	Object floor = {
		OBJECT_FLOOR,
		0,
		(vec2){0, 200},
		(vec2){0, 0},
		64, 64, 64,
		512, 64,
		true, true, false
	};

	Object wall = {
		OBJECT_FLOOR,
		0,
		(vec2){300, 72},
		(vec2){0, 0},
		64, 64, 64,
		64, 128,
		true, true, false
	};

	while (1) {
		// Walking
		int dx = 0;
		if ( button_pressed(0, BTN_LEFT) )
			dx--;
		if ( button_pressed(0, BTN_RIGHT) )
			dx++;

		player_walk(&player, dx);

		if ( button_pressed(0, BTN_CROSS) && player.on_floor ) {
			player.velocity.y -= 100;
		}

		player_gravity(&player);
		player_update(&player);

		player_collision(&player, &floor);
		player_collision(&player, &wall);

		// player_update(&player);

		// player_walk(&player, dx);
		// player_gravity(&player);
		// player_walk(&player, dx);

		FntLoad(960, 0);
		FntOpen(MARGINX+x, MARGINY+y, SCREENXRES - MARGINX * 2, FONTSIZE, 0, 1024 );
		// player.on_floor ? FntPrint("ON FLOOR") : FntPrint("NOT ON FLOOR");
		// FntFlush(-1);

		player_render(&player);
		render_object(&floor);
		render_object(&wall);

		GsSetOrign(-player.position.x+CENTERX, -player.position.y+CENTERY);

		display();

	}

	return 0;

}
