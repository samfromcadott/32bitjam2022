#include <stdlib.h>

#include "vector.h"
#include "util.h"

inline int min(const int a, const int b) {
	return (a) < (b) ? (a) : (b);
}

inline int max(const int a, const int b) {
	return (a) > (b) ? (a) : (b);
}

inline int sign(int x) {
	return (x >= 0) - (x < 0);
}

inline int clamp(int n, int min, int max) {
	const int t = n < min ? min : n;
	return t > max ? max : t;
}

inline vec2 overlap(int w0, int h0, int x0, int y0, int w1, int h1, int x1, int y1) {
	vec2 distance, o;

	distance.x = abs(x0 - x1);
	distance.y = abs(y0 - y1);
	o.x = w0/2 + w1/2 - distance.x;
	o.y = h0/2 + h1/2 - distance.y;

	return o;

}
