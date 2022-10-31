#include <sys/types.h>
#include <stdbool.h>
#include <stdio.h>
#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>

#include "file.h"
#include "texture.h"

GsIMAGE load_texture(char *filename) {
	u_long* data = load_file(filename);
	GsIMAGE image;
	GsGetTimInfo(data, &image);

	return image;

}

u_short use_image(GsIMAGE *image) {
	RECT pixel_rect = {
		image->px, image->py,
		image->pw, image->ph,
	};

	RECT color_rect = {
		image->cx, image->cy,
		image->cw, image->ch,
	};

	// Upload pixel data to framebuffer
	LoadImage(&pixel_rect, image->pixel);
	DrawSync(0);

	// Upload CLUT to framebuffer if present
	if ( image->pmode & 0x8 ) {
		LoadImage(&color_rect, image->clut);
		DrawSync(0);

	}

	return GetTPage( (image->pmode & 3), 0, image->px, image->py );

}
