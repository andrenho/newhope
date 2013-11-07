#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdint.h>

void if_init();
uint8_t if_world_tile_stack(int x, int y, uint16_t stack[10]);

#endif
