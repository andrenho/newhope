#include "util/geometry.h"

bool intersect(Rect r1, Rect r2)
{
	if((r1.x >= r2.x && r1.y >= r2.y && r1.x < (r2.x+r2.w) && r1.y < (r2.y+r2.h))
	|| (r1.x >= r2.x && r1.y >= r2.y && r1.x < (r2.x+r2.w) && r1.y < (r2.y+r2.h))) {
	}
}
