#pragma once

#include <sys/types.h>
#include <stdbool.h>
#include <stdio.h>
#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>

GsIMAGE load_texture(char *filename);
u_short use_image(GsIMAGE *image);
