#pragma once

typedef struct {
	short x, y;
} vec2;

inline vec2 vec_add(const vec2 a, const vec2 b) {
	return {a.x + b.x, a.y + b.y};
}

inline vec2 vec_sub(const vec2 a, const vec2 b) {
	return {a.x - b.x, a.y - b.y};
}

inline vec2 vec_scale(const vec2 v, const short s) {
	return {v.x * s, v.y * s};
}
