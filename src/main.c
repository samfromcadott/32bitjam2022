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

	// Object player = {
	// 	OBJECT_PLAYER,
	// 	0,
	// 	(vec2){32, 32},
	// 	(vec2){0, 0},
	// 	0, 127, 192,
	// 	32, 48,
	// 	true, true, true
	// };

	Player player = {
		0,
		(vec2){32, 32},
		(vec2){0, 0},
		5,
		2, 2,
		2, 2,
		2, 2,
		0, 127, 192,
		32, 48,
		true, false, true
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

	while (1) {
		// FntLoad(960, 0);
		// FntOpen(MARGINX+x, MARGINY+y, SCREENXRES - MARGINX * 2, FONTSIZE, 0, 1024 );
		// FntPrint("%s", text_file);
		// FntFlush(-1);
		// player.velocity = (vec2){0, 0};
		// player.velocity.x = 0;
		// player.velocity.x /= 2;
		// player.gravity = true;
		// bool collision, want_jump;
		// int player_speed = 5;
		// if ( !collision ) player_speed = 2;

		// Walking
		// if ( button_pressed(0, BTN_LEFT) )
		// 	player.velocity.x += max(-player_speed, player.velocity.x-player_speed);
		// if ( button_pressed(0, BTN_RIGHT) )
		// 	player.velocity.x += min(+player_speed, player.velocity.x+player_speed);
		if ( button_pressed(0, BTN_LEFT) )
			player_walk(&player, -1);
		if ( button_pressed(0, BTN_RIGHT) )
			player_walk(&player, +1);
		if ( !button_pressed(0, BTN_LEFT) && !button_pressed(0, BTN_RIGHT) )
			player_walk(&player, 0);

		// Jumping
		// if ( button_pressed(0, BTN_CROSS) && collision )
		// 	// want_jump = true;
		// 	player.velocity.y = -50;

		// if ( !button_pressed(0, BTN_CROSS) && want_jump && collision ) {
		// 	player.velocity.y -= 15;
		// 	want_jump = false;
		// }

		// move_object(&player);
		//
		// collision = (
		// 	player.position.x <= floor.position.x + floor.width &&
		// 	player.position.x + player.width >= floor.position.x &&
		// 	player.position.y <= floor.position.y + floor.height &&
		// 	player.position.y + player.height >= floor.position.y
		// );
		// if ( collision ) {
		// 	player.gravity = false;
		// 	player.velocity.y = 0;
		// 	player.position.y = floor.position.y - player.height;
		// }

		player_update(&player);



		player_render(&player);
		render_object(&floor);

		GsSetOrign(-player.position.x+CENTERX, -player.position.y+CENTERY);

		display();

	}

	return 0;

}
