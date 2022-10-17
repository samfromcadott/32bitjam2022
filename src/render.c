#include <sys/types.h>
#include <stdio.h>
#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>

#include "render.h"

void init_display() {
	SetVideoMode(MODE_NTSC);
	GsInitGraph(SCREENXRES, SCREENYRES, GsINTER | GsOFSGPU, 1, 0);
	GsDefDispBuff(0, 0, 0, 0);
	GsInitVcount();

	SetDispMask(1);

	// Initialize OT
	OT.length = OTLEN;
	OT.org = (GsOT_TAG*)ot;
	GsSetWorkBase(pribuff);

}

void display() {
	DrawSync(0);
	VSync(0);
	GsSwapDispBuff();

	GsDrawOt(&OT);

	GsSetWorkBase(pribuff);
	GsClearOt(0, 0, &OT);
	GsSortClear(128, 128, 128, &OT);

}
