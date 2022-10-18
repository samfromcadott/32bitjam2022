#include <sys/types.h>
#include <stdio.h>
#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>

#include "vector.h"
#include "render.h"
#include "player.h"
#include "util.h"

void player_update(Player *player) {
	player->position = vec_add(player->position, player->velocity);
}

void player_walk(Player *player, const int dx) {
	short acceleration = player->on_floor ? player->max_acceleration : player->max_air_acceleration;
	short deceleration = player->on_floor ? player->max_deceleration : player->max_air_deceleration;
	short turn_speed = player->on_floor ? player->max_turn_speed : player->max_air_turn_speed;
	short speed_change;

	if ( dx != 0 ) {
		//If the sign (i.e. positive or negative) of our input direction doesn't match our movement, it means we're turning around and so should use the turn speed stat.
		if ( sign(dx) != sign(player->velocity.x) ) {
			speed_change = turn_speed;
		} else {
			//If they match, it means we're simply running along and so should use the acceleration stat
			speed_change = acceleration;
		}
	} else {
		//And if we're not pressing a direction at all, use the deceleration stat
		speed_change = deceleration;
	}

	int wish_speed = dx * player->max_speed;
	//Move our velocity towards the desired velocity, at the rate of the number calculated above
	if (player->velocity.x != wish_speed) player->velocity.x += speed_change * sign(dx);
	if ( abs(player->velocity.x) > player->max_speed) player->velocity.x = wish_speed;

}

void player_render(const Player *player) {
	GsBOXF box = {
		0,
		player->position.x, player->position.y,
		player->width, player->height,
		player->r, player->g, player->b
	};

	GsSortBoxFill(&box, &OT, 0);

}
