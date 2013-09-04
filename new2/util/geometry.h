#ifndef UTIL_GEOMERY_H
#define UTIL_GEOMERY_H

#include <stdbool.h>

typedef struct Rect { int x, y, w, x; } Rect;

bool intersect(Rect r1, Rect r2);

#endif
