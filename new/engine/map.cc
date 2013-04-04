#include "defines.h"

#include <cstdlib>

Map::Map() : w_(0), h_(0) 
{
	for(int i(0); i<100; i++) {
		for(int j(0); j<100; j++) {
			randspecial_[i][j] = rand() % 200;
		}
	}
}


int 
Map::Special(int x, int y) const
{
	if(x < 0 || y < 0 || x >= w_ || y >= h_) {
		return 0; // no special
	}
	int s = randspecial_[x % 100][y % 100];
	if(s >= 5) {
		return 0;
	}
	return s + 1;
}
