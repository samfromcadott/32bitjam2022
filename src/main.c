#include <sys/types.h>
#include <stdbool.h>
#include <stdio.h>
#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libcd.h>
#include <malloc.h>

#include "file.h"
#include "render.h"
#include "input.h"
#include "objects.h"
#include "vector.h"
#include "util.h"
#include "player.h"

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

int main() {
	init();
	short x = 32, y =32;

	char* text_file = (char*)load_file("\\ANNABEL.TXT;1");

	GsClearOt(0, 0, &OT);
	GsSortClear(64, 64, 128, &OT);

	Player player = {
		0,
		(vec2){300, -20},
		(vec2){0, 0},
		20,
		5, 5,
		5, 5,
		2, 2,
		10,
		0, 127, 192,
		32, 48,
		true, false, false, false
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

	Object ceiling = {
		OBJECT_FLOOR,
		0,
		(vec2){0, 50},
		(vec2){0, 0},
		64, 64, 64,
		128, 64,
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

		// if ( button_pressed(0, BTN_CROSS) && player.on_floor ) {
		// 	player.velocity.y -= 100;
		// }

		player_gravity(&player);

		if ( button_pressed(0, BTN_CROSS) && player.on_floor && !player.is_jumping ) {
			// player.velocity.y = -10;
			player_jump(&player);
		}

		if ( !button_pressed(0, BTN_CROSS) ) {
			player.is_jumping = false;
		}

		player_update(&player);

		// if ( button_pressed(0, BTN_CROSS) ) {
		// 	player.velocity.y -= 10;
		// }

		player.on_floor = false; player.on_ceiling = false; player.on_wall = false;
		player_collision(&player, &floor);
		player_collision(&player, &wall);
		player_collision(&player, &ceiling);

		// if ( button_pressed(0, BTN_CROSS) ) {
		// 	player.position.y -= 10;
		// }

		FntLoad(960, 0);
		FntOpen(MARGINX+x, MARGINY+y, SCREENXRES - MARGINX * 2, FONTSIZE, 0, 1024 );
		player.on_floor ? FntPrint("ON FLOOR\n") : FntPrint("NOT ON FLOOR\n");
		player.on_wall ? FntPrint("ON WALL\n") : FntPrint("NOT ON WALL\n");
		player.on_ceiling ? FntPrint("ON CEILING\n") : FntPrint("NOT ON CEILING\n");
		FntFlush(-1);

		player_render(&player);
		render_object(&floor);
		render_object(&wall);
		render_object(&ceiling);

		GsSetOrign(-player.position.x+CENTERX, -player.position.y+CENTERY);

		display();

	}

	return 0;

}
