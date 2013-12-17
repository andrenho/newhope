#include "timer.h"

#include "SDL_timer.h"

Timer* timer_init(int argc, char** argv)
{
	Timer* t = malloc(sizeof(Timer));

	for(int i=0; i<argc; i++) {
		t->active = !strcmp(argv[i], "-t");
	}
	t->avg = 0;
	t->max = 0;
	t->_count = 1;
	t->_time = 0;
	return t;
}


void timer_start(Timer* t)
{
	if(t->active) {
		t->_time = SDL_GetTicks();
	}
}


void timer_stop(Timer* t)
{
	if(t->active) {
		uint32_t g = SDL_GetTicks() - t->_time;
		if(g > t->max)
			t->max = g;
		t->avg = (t->avg * (float)(t->_count-1) / (float)t->_count) +
			 (g * 1.0f / (float)t->_count);
		t->_count++;
	}
}


void timer_finish(Timer* t)
{
	if(t->active) {
		printf("Timer average: %0.2fms\n", t->avg);
		printf("Timer max: %d.00ms\n", t->max);
	}
	free(t);
}
