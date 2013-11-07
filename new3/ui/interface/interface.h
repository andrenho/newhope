#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdint.h>

typedef uint16_t BLOCK;

void if_init();
void if_finish();

void if_next_frame();
void if_hero_position(double* x, double* y);
uint8_t if_world_tile_stack(int x, int y, BLOCK stack[10]);

#endif
