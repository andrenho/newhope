#include "engine/person.h"

#include <stdlib.h>

#include "engine/world.h"

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
	double fx = p->x, fy = p->y, step = 0.0;
	if(p->speed == WALKING) {
		step = .1;
	} else if(p->speed == RUNNING) {
		step = .2;
	}

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

	p->x = fx;
	p->y = fy;
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
