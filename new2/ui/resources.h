#ifndef UI_RESOURCES_H
#define UI_RESOURCES_H

#include "SDL2/SDL.h"

enum Resource { R_GRASS, N_RESOURCES };

typedef struct Resources {
	SDL_Texture** texture;
} Resources;

Resources* resources_init();
void resources_free(Resources** r);

#endif
