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

// char pribuff[2][32768];
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
	// DrawOTag(ot[db]+OTLEN-1);   // Draw the ordering table
	// db = !db;                   // Swap buffers on every pass (alternates between 1 and 0)
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

void LoadTexture(u_long *tim, TIM_IMAGE *tparam) {
	// Read TIM information
	OpenTIM(tim);
	ReadTIM(tparam);

	// Upload pixel data to framebuffer
	LoadImage(tparam->prect, (u_long*)tparam->paddr);
	DrawSync(0);

	// Upload CLUT to framebuffer if present
	if( tparam->mode & 0x8 ) {
		LoadImage(tparam->crect, (u_long*)tparam->caddr);
		DrawSync(0);

	}

}

void draw_image(SPRT *sprt) {
	ClearOTagR(ot[db], OTLEN);      // Clear ordering table

	// Sort textured sprite

	// sprt = (SPRT*)nextpri;

	setSprt(sprt);                  // Initialize the primitive (very important)
	setXY0(sprt, 48, 48);           // Position the sprite at (48,48)
	setWH(sprt, 64, 64);            // Set size to 64x64 pixels
	setUV0(sprt,                    // Set UV coordinates
		tim_uoffs,
		tim_voffs);
	setClut(sprt,                   // Set CLUT coordinates to sprite
		tim_crect.x,
		tim_crect.y);
	setRGB0(sprt,                   // Set primitive color
		128, 128, 128);
	addPrim(ot[db], sprt);          // Sort primitive to OT

	// nextpri += sizeof(SPRT);        // Advance next primitive address

}


void loadstuff(void) {

	TIM_IMAGE my_image;         // TIM image parameters

	// extern u_long tim_my_image[]; // TODO: load image from disk here
	u_long *tim_my_image = load_file("\\TX64.TIM;1");

	// Load the texture
	LoadTexture(tim_my_image, &my_image);

	// Copy the TIM coordinates
	tim_prect   = *my_image.prect;
	tim_crect   = *my_image.crect;
	tim_mode    = my_image.mode;

	// Calculate U,V offset for TIMs that are not page aligned
	tim_uoffs = (tim_prect.x%64)<<(2-(tim_mode&0x3));
	tim_voffs = (tim_prect.y&0xff);

}

int main() {
	init();
	short x = 32, y =32;

	// SPRT *sprt;
	// TILE *tile;

	// loadstuff();

	// GsDRAWENV.tpage = getTPage( tim_mode&0x3, 0, tim_prect.x, tim_prect.y );
	// draw[1].tpage = getTPage( tim_mode&0x3, 0, tim_prect.x, tim_prect.y );

	char* text_file = (char*)load_file("\\ANNABEL.TXT;1");

	GsClearOt(0, 0, &OT);
	GsSortClear(64, 64, 128, &OT);

	while (1) {
		// GsClearDispArea(64, 64, 128);
		// GsSetWorkBase(pribuff);
		// GsClearOt(0, 0, &OT);
		// GsSortClear(64, 64, 128, &OT);

		FntLoad(960, 0);
		FntOpen(MARGINX+x, MARGINY+y, SCREENXRES - MARGINX * 2, FONTSIZE, 0, 1024 );
		FntPrint("%s", text_file);
		FntFlush(-1);

		// draw_image(sprt);

		// ClearOTagR(ot[db], OTLEN);  // Clear ordering table
		// tile = (TILE*)nextpri;         // Cast next primitive
		//
		// setTile(tile);                 // Initialize the primitive (very important)
		// setXY0(tile, 32+x, 32);          // Set primitive (x,y) position
		// setWH(tile, 64, 64);           // Set primitive size
		// setRGB0(tile, 255, 255, 0);    // Set color yellow
		//
		// addPrim(ot[db], tile);         // Add primitive to the ordering table

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
