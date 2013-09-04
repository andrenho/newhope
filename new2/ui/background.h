#ifndef UI_BACKGROUND_H
#define UI_BACKGROUND_H

#include "SDL2/SDL.h"

struct UI;

typedef struct Background {
	struct UI* ui;
	SDL_Texture* tx;
} Background;

Background* bg_init();
void bg_free(Background** b);

void bg_recreate(Background* b);
void bg_render(Background* b);

#endif
