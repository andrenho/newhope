#ifndef UI_RESOURCES_H
#define UI_RESOURCES_H

#include <stdint.h>
#include <stdbool.h>

#include "SDL2/SDL.h"
#include "block.h"

bool resources_init(SDL_Renderer* ren);
void resources_free();

SDL_Rect resources_terrain_rect(uint16_t t);

#endif
