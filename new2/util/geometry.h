#ifndef UTIL_GEOMERY_H
#define UTIL_GEOMERY_H

#include <stdbool.h>

typedef struct Rect { int x, y, w, h; } Rect;
typedef struct Point { int x, y; } Point;

bool intersect_rect(Rect r1, Rect r2);
bool intersect_point(Rect r, Point p);

#endif
