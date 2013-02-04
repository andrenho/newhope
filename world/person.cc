#include "world/person.h"

#include "util/logger.h"

Person::~Person()
{
}


void
Person::Process()
{
	if(!(mov_x_ == 0 && mov_y_ == 0)) {
		pos_.x += mov_x_ / 4.0;
		pos_.y += mov_y_ / 4.0;
		++step_;
	}
}


void 
Person::Movement(int x, int y)
{
	if(mov_x_ == x && mov_y_ == y) {
		return;
	}

	mov_x_ = x; 
	mov_y_ = y;
	step_ = 0;

	if(x == 1) {
		facing_ = 'e';
	} else if(x == -1) {
		facing_ = 'w';
	} else if(y == 1) {
		facing_ = 's';
	} else if(y == -1) {
		facing_ = 'n';
	}
}
