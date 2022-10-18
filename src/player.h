#pragma once

#include <stdbool.h>

#include "vector.h"

typedef struct {
	u_short timer;
	vec2 position;
	vec2 velocity;
	short max_speed;
	short max_acceleration, max_deceleration;
	short max_air_acceleration, max_air_deceleration;
	short max_turn_speed, max_air_turn_speed;
	u_short r, g, b;
	u_short width, height;
	bool on_floor, on_wall, on_ceiling;
} Player;

void player_update(Player *player);
void player_walk(Player *player, const int dx);
void player_render(const Player *player);
