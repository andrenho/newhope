#ifndef UI_RESOURCES_H
#define UI_RESOURCES_H

#include "SDL2/SDL.h"

#include "engine/terrain.h"

enum Resource { R_GRASS, N_RESOURCES };

typedef struct Resources {
	SDL_Texture** texture;
} Resources;

Resources* resources_init(SDL_Renderer* ren);
void resources_free(Resources** r);
SDL_Texture* resources_terrain_texture(Resources* r, Terrain t);

#endif
