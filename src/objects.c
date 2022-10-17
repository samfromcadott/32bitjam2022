#include <sys/types.h>
#include <stdio.h>
#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>

#include "render.h"
#include "objects.h"

void render_object(const Object *object) {
	GsBOXF box = {
		0,
		object->position.x, object->position.y,
		object->width, object->height,
		object->r, object->g, object->b
	};

	GsSortBoxFill(&box, &OT, 0);

}

void move_object(Object *object) {
	if ( object->gravity ) object->velocity.y += gravity;
	object->position = vec_add(object->position, object->velocity);

}
