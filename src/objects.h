#pragma once

#include <stdbool.h>

#include "vector.h"

enum {
	OBJECT_NULL,
	OBJECT_PLAYER,
	OBJECT_FLOOR
};

typedef struct {
	u_short type;
	u_short timer;
	vec2 position;
	vec2 velocity;
	u_short r, g, b;
	u_short width, height;
	bool active, solid;
} Object;

void render_object(const Object *object);
void move_object(Object *object);
