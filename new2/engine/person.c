#include "engine/person.h"

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "engine/world.h"


static bool person_can_move(Person* p, World* w, double x, double y);


Person* person_init(double x, double y)
{
	Person* p = calloc(sizeof(Person), 1);
	p->x = x;
	p->y = y;
	p->direction = 90;
	p->speed = STOPPED;
	return p;
}


void person_free(Person** p)
{
	free(*p);
	*p = NULL;
}


void person_step(Person* p, World* w)
{
	// set speed
	double fx = p->x, fy = p->y, step = 0.0;
	if(p->speed == WALKING) {
		step = .08;
	} else if(p->speed == RUNNING) {
		step = .15;
	} else {
		return;
	}

	// set direction
	switch(p->direction) {
	case   0: fy -= step; break;
	case  45: fx += step; fy -= step; break;
	case  90: fx += step; break;
	case 135: fx += step; fy += step; break;
	case 180: fy += step; break;
	case 225: fx -= step; fy += step; break;
	case 270: fx -= step; break;
	case 315: fx -= step; fy -= step; break;
	default: abort();
	}

	// if the person can't move diagonally, try moving horizontal/vertical
	if(fx != 0 && fy != 0 && !person_can_move(p, w, fx, fy)) {
		if(person_can_move(p, w, fx, p->y)) {
			fy = p->y;
		} else if(person_can_move(p, w, p->x, fy)) {
			fx = p->x;
		}
	}

	// check if the person can move
	if(person_can_move(p, w, fx, fy)) {
		p->x = fx;
		p->y = fy;
	}
}


void person_start_running(Person* p, int direction)
{
	p->direction = direction;
	p->speed = RUNNING;
}


void person_stop_running(Person* p)
{
	p->speed = STOPPED;
}


/********************
 *                  *
 * STATIC FUNCTIONS *
 *                  *
 ********************/


static bool person_can_move(Person* p, World* w, double x, double y)
{
	if(!world_tile_walkable(w, floor(x-0.5), floor(y-0.5))
	|| !world_tile_walkable(w, floor(x+0.5), floor(y-0.5))
	|| !world_tile_walkable(w, floor(x-0.5), floor(y+0.5))
	|| !world_tile_walkable(w, floor(x+0.5), floor(y+0.5))) {
		return false;
	}

	return true;
}
