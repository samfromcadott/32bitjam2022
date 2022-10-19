#include <sys/types.h>
#include <stdio.h>
#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>

#include "vector.h"
#include "render.h"
#include "player.h"
#include "objects.h"
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
		// If the sign of dx and velocity.x are inequal the player is trying to change direction
		if ( sign(dx) != sign(player->velocity.x) ) {
			speed_change = turn_speed;
		} else {
			speed_change = acceleration; // Continuing in same direction
		}
	} else {
		speed_change = deceleration; // Decelerate if no input
	}

	// Move velocity towards target velocity
	int wish_speed = dx * player->max_speed;

	if (player->velocity.x < wish_speed) player->velocity.x += speed_change;
	if (player->velocity.x > wish_speed) player->velocity.x -= speed_change;
	if ( abs(player->velocity.x) > player->max_speed ) player->velocity.x = wish_speed;

}

void player_gravity(Player *player) {
	//We change the character's gravity based on her Y direction
	int grav_multiplier = 1;

	// If Kit is going up...
	if (player->velocity.y < 0) {
		if (player->on_floor) {
			//Don't change it if Kit is stood on something (such as a moving platform)
			grav_multiplier = 0;
		} else {
				// //Apply upward multiplier if player is rising and holding jump
				// if (pressingJump && currentlyJumping)
				// {
				// 	grav_multiplier = upwardMovementMultiplier;
				// }
				// //But apply a special downward multiplier if the player lets go of jump
				// else
				// {
				// 	grav_multiplier = jumpCutOff;
				// }

		}
	}

	//Else if going down...
	else if (player->velocity.y > 0) {

		if (player->on_floor) {
		//Don't change it if Kit is stood on something (such as a moving platform)

			grav_multiplier = 0;
		} else {

			//Otherwise, apply the downward gravity multiplier as Kit comes back to Earth
			// grav_multiplier = downwardMovementMultiplier;
			grav_multiplier = 2;
		}

	}
	//Else not moving vertically at all
	else {
		if (player->on_floor) {
			grav_multiplier = 0;
		}

		grav_multiplier = 1;
	}

	//Set the character's Rigidbody's velocity
	//But clamp the Y variable within the bounds of the speed limit, for the terminal velocity assist option
	player->velocity.y = gravity * grav_multiplier;
	player->velocity.y = clamp(player->velocity.y, -100, terminal_velocity);
}

void player_collision(Player *player, Object *object) {
	bool collided = collision(player->width, player->height, player->position.x, player->position.y,
	object->width, object->height, object->position.x, object->position.y);

	if (!collided) {
		player->on_floor = false; player->on_ceiling = false; player->on_wall = false;
		return;
	}

	// From above
	if (player->velocity.y > 0 && player->position.y < object->position.y) {
		player->position.y = object->position.y - player->height;
		player->on_floor = true;
	}

	// From below
	else if (player->velocity.y < 0 && player->position.y > object->position.y) {
		player->position.y = object->position.y + object->height;
		player->on_ceiling = true;
	}

	// From left
	else if (player->velocity.x > 0 && player->position.x < object->position.x) {
		player->position.x = object->position.x - player->width;
		player->on_wall = true;
	}

	// From right
	else if (player->velocity.x < 0 && player->position.x > object->position.x) {
		player->position.x = object->position.x + object->width;
		player->on_wall = true;
	}

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
