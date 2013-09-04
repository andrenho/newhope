#ifndef UI_UI_H
#define UI_UI_H

#include <stdbool.h>
#include <stdint.h>
#include "SDL2/SDL.h"

#include "ui/resources.h"
#include "ui/background.h"

struct World;

#define TILE_W 16
#define TILE_H 16

#define FPS 60

typedef struct UI {
	struct World* w;
	SDL_Window* win;
	SDL_Renderer* ren;
	bool active;
	Resources* res;
	int rx, ry;
	uint32_t last_frame;
	Background* bg;
} UI;

UI* ui_init(struct World* w);
void ui_free(UI** u);

bool ui_active(UI* u);
void ui_do_events(UI* u);
void ui_render(UI* u);
void ui_wait_next_frame(UI* u);

void ui_screen_limits(UI* u, int* x1, int* y1, int* x2, int* y2);

#endif
