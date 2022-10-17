#include <sys/types.h>
#include <stdio.h>
#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libcd.h>
#include <malloc.h>

#define CD_SECTOR_SIZE 2048
// Bytes to sectors
#define BtoS(len) ( ( len + CD_SECTOR_SIZE - 1 ) / CD_SECTOR_SIZE )

#define SCREENXRES 640 // Screen width
#define SCREENYRES 480 // Screen height
#define CENTERX SCREENXRES/2 // Center of screen on x
#define CENTERY SCREENYRES/2 // Center of screen on y
#define MARGINX 16 // margins for text display
#define MARGINY 16
#define FONTSIZE 8 * 60 // Text Field Height
static unsigned char heap[0x40000];

#define OTLEN 8

GsOT OT;
u_long ot[2][OTLEN];
int db = 0;

char pribuff[256];

char *nextpri;          // Next primitive pointer

int tim_mode;           // TIM image parameters
RECT tim_prect,tim_crect;
int tim_uoffs,tim_voffs;

void init() {
	SetVideoMode(MODE_NTSC);
	GsInitGraph(SCREENXRES, SCREENYRES, GsINTER | GsOFSGPU, 1, 0);
	GsDefDispBuff(0, 0, 0, 0);
	GsInitVcount();

	SetDispMask(1);

	// Initialize CD and heap
	CdInit();
	InitHeap((u_long *)heap, sizeof(heap));

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
	GsSortClear(64, 64, 128, &OT);

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
		FntLoad(960, 0);
		FntOpen(MARGINX+x, MARGINY+y, SCREENXRES - MARGINX * 2, FONTSIZE, 0, 1024 );
		FntPrint("%s", text_file);
		FntFlush(-1);

		GsBOXF tile = {
			0,
			x, y,
			64, 64,
			255, 255, 0
		};
		GsSortBoxFill(&tile, &OT, 0);

		x = (x+1) % SCREENXRES;
		y = (y+1) % SCREENYRES;

		display();

	}

	return 0;

}
