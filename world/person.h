#ifndef WORLD_PERSON_H
#define WORLD_PERSON_H

#include <vector>

#include "util/defines.h"
#include "util/point.h"
class World;

class Person {
public:
	Person(const World& world, Tile pos)
		: pos_(pos), facing_('s'), world_(world), mov_x_(0), mov_y_(0),
	          step_(0) { }
	~Person();

	void Process();
	void Movement(int x, int y);

	inline const int step() const { return step_; }
	inline const Tile pos() const { return pos_; }
	inline const char facing() const { return facing_; }

private:
	Tile pos_;
	char facing_;
	const World& world_;
	int mov_x_, mov_y_;
	int step_;

	DISALLOW_COPY_AND_ASSIGN(Person);
};

#endif
