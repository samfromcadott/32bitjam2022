#pragma once

inline int min(const int a, const int b) {
	return (a) < (b) ? (a) : (b);
}

inline int max(const int a, const int b) {
	return (a) > (b) ? (a) : (b);
}

inline int sign(int x) {
	return (x >= 0) - (x < 0);
}
