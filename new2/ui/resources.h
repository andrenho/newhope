#ifndef UI_RESOURCES_H
#define UI_RESOURCES_H

#include "SDL2/SDL.h"

#include "engine/terrain.h"

typedef struct Resources {
	SDL_Texture* sprites;
} Resources;

Resources* resources_init(SDL_Renderer* ren);
void resources_free(Resources** r);
SDL_Rect resources_terrain_rect(Resources* r, Terrain t);

#endif
