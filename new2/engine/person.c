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
	double step = 0.0;
	if(p->speed == WALKING) {
		step = .08;
	} else if(p->speed == RUNNING) {
		step = .15;
	}

	switch(p->direction) {
	case   0: p->y -= step; break;
	case  45: p->x += step; p->y -= step; break;
	case  90: p->x += step; break;
	case 135: p->x += step; p->y += step; break;
	case 180: p->y += step; break;
	case 225: p->x -= step; p->y += step; break;
	case 270: p->x -= step; break;
	case 315: p->x -= step; p->y -= step; break;
	default: abort();
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
