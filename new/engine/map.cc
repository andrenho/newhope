#include "defines.h"

#include <cstdlib>

Map::Map() : w_(0), h_(0) 
{
	for(int i=0; i<100; i++) {
		for(int j=0; j<100; j++) {
			randspecial_[i][j] = rand() % 198;
		}
	}
}


int 
Map::Special(Point<int> p) const
{
	if(p.x < 0 || p.y < 0 || p.x >= w_ || p.y >= h_) {
		return 0; // no special
	}
	int s = randspecial_[p.x % 100][p.y % 100];
	if(s >= 5) {
		return 0;
	}
	return s + 1;
}
