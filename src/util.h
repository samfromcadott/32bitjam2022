#pragma once

#include <stdbool.h>

#include "vector.h"

int min(const int a, const int b);
int max(const int a, const int b);
int sign(int x);
int clamp(int n, int min, int max);
vec2 overlap(int w0, int h0, int x0, int y0, int w1, int h1, int x1, int y1);
bool collision(int w0, int h0, int x0, int y0, int w1, int h1, int x1, int y1);
