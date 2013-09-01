#ifndef UI_UI_H
#define UI_UI_H

#include <stdbool.h>
#include "SDL2/SDL.h"

#include "ui/resources.h"

typedef struct UI {
	SDL_Window* win;
	SDL_Renderer* ren;
	bool active;
	Resources* res;
} UI;

UI* ui_init();
void ui_free(UI** u);

bool ui_active(UI* u);
void ui_do_events(UI* u);
void ui_render(UI* u);

#endif
