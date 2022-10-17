#include <sys/types.h>
#include <stdio.h>
#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>

#include "render.h"
#include "objects.h"
#include "util.h"

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
	// object->position = vec_add(object->position, object->velocity);
	object->position.x += object->velocity.x;
	// object->position.y += object->velocity.y;
	object->position.y += object->velocity.y;
	// if ( object->gravity ) object->velocity.y += gravity;
	if ( object->gravity ) object->velocity.y = min(gravity, object->velocity.y + gravity);

}
