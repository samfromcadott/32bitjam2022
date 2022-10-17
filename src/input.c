#include <sys/types.h>
#include <stdio.h>
#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libapi.h>
#include <stdbool.h>

#include "input.h"

void init_input() {
	// Initialize the pads
	InitPAD( padbuff[0], 34, padbuff[1], 34 );

	// Begin polling
	StartPAD();

}

bool button_pressed(const int controller, const u_short button) {
	PADTYPE* pad = (PADTYPE*)padbuff[controller];

	// Only parse inputs when a controller is connected
	if ( pad->stat != 0 ) return false;

	// Only parse when a digital pad,
	// dual-analog and dual-shock is connected
	if (
		( pad->type != 0x4 ) &&
		( pad->type != 0x5 ) &&
		( pad->type != 0x7 )
	) return false;

	if ( pad->btn&button ) return false;

	return true;

}
