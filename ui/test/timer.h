#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>
#include <stdint.h>

typedef struct Timer {
	bool active;
	float avg;
	uint32_t max;
	uint32_t _count;
	uint32_t _time;
} Timer;

Timer* timer_init(int argc, char** argv);
void timer_start(Timer* t);
void timer_stop(Timer* t);
void timer_finish(Timer* t);

#endif
