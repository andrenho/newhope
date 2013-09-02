#ifndef UI_UI_H
#define UI_UI_H

#include <stdbool.h>
#include <stdint.h>
#include "SDL2/SDL.h"

#include "ui/resources.h"

struct World;

#define TILE_W 16
#define TILE_H 16

#define FPS 50

typedef struct UI {
	SDL_Window* win;
	SDL_Renderer* ren;
	bool active;
	Resources* res;
	int rx, ry;
	uint32_t last_frame;

	SDL_Texture* bg;
	SDL_Renderer* ren_bg;
} UI;

UI* ui_init();
void ui_free(UI** u);

bool ui_active(UI* u);
void ui_do_events(UI* u, struct World* w);
void ui_render(UI* u, struct World* w);
void ui_wait_next_frame(UI* u);

#endif
