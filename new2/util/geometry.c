#include "util/geometry.h"

bool intersect_rect(Rect r1, Rect r2)
{
	if((r1.x >= r2.x && r1.y >= r2.y && r1.x < (r2.x+r2.w) && r1.y < (r2.y+r2.h))
	|| (r2.x >= r1.x && r2.y >= r1.y && r2.x < (r1.x+r1.w) && r2.y < (r1.y+r1.h))) {
		return true;
	}
	return false;
}


bool intersect_point(Rect r, Point p)
{
	if(p.x >= r.x && p.x <= (r.x+r.w) && p.y >= r.y && p.y <= (r.y+r.h)) {
		return true;
	}
	return false;
}
