#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdint.h>

typedef uint16_t BLOCK;

typedef struct Person {
	double x, y;
	double direction;
	int image;
} Person;

//
// initialization
//
void if_init();
void if_finish();

//
// requests
//
void if_next_frame();
void if_hero_move(int speed, double direction);

//
// queries
//
void if_hero_position(double* x, double* y);
int if_people_visible(int x1, int y1, int x2, int y2, Person** people);
uint8_t if_world_tile_stack(int x, int y, BLOCK stack[10]);

#endif
