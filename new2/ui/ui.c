#include "ui/ui.h"

#include <stdlib.h>
#include <stdio.h>

#include "SDL2/SDL.h"

#include "engine/world.h"


static bool ui_sdl_init(UI* u);
static void ui_sdl_end(UI* u);
static void ui_draw_terrain(UI* u, World* w, int x, int y, Terrain t);
static void ui_screen_limits(int* x1, int* y1, int* x2, int* y2);


// create the UI object and initialize the user interface
UI* ui_init()
{
	UI* u = malloc(sizeof(UI));

	if(!ui_sdl_init(u))
		return NULL;

	u->active = true;
	u->res = resources_init(u->ren);
	if(!u->res)
		return NULL;
	u->rx = 0;
	u->ry = 0;
	
	return u;
}


// free the UI object
void ui_free(UI** u)
{
	resources_free(&(*u)->res);
	ui_sdl_end(*u);
	free(*u);
	*u = NULL;
}


// return if the UI is still active
bool ui_active(UI* u)
{
	return u->active;
}


// parse user input and send the events to the engine
void ui_do_events(UI* u, World* w)
{
	SDL_Event e;
	while(SDL_PollEvent(&e)) {
		switch(e.type) {
		case SDL_QUIT:
			u->active = false;
			break;
		}
	}
}


// present the image to the user
void ui_render(UI* u, World* w)
{
	int x1, y1, x2, y2;
	ui_screen_limits(&x1, &y1, &x2, &y2);

	for(int x=x1; x<=x2; x++) {
		for(int y=y1; y<=y2; y++) {
			Object obj;
			Terrain t = world_xy(w, x, y, &obj);
			ui_draw_terrain(u, w, x, y, t);
		}
	}
	SDL_RenderPresent(u->ren);
}


/********************
 *                  *
 * STATIC FUNCTIONS *
 *                  *
 ********************/
static bool ui_sdl_init(UI* u)
{
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "error: unable to initialize SDL: %s\n", 
				SDL_GetError());
		return false;
	}

	if(SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_RESIZABLE, &u->win,
				&u->ren) != 0) {
		SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "error initializing "
				"window: %s\n", SDL_GetError());
		return false;
	}
	SDL_SetWindowTitle(u->win, "New Hope " VERSION);

	return true;
}


static void ui_sdl_end(UI* u)
{
	SDL_DestroyRenderer(u->ren);
	SDL_DestroyWindow(u->win);
	SDL_Quit();
}


static void ui_draw_terrain(UI* u, World* w, int x, int y, Terrain t)
{
	SDL_Rect rs = resources_terrain_rect(u->res, t);
	SDL_Rect rd = { .x = (x*TILE_W) - u->rx, .y = (y*TILE_H) - u->ry,
	               .w = TILE_W, .h = TILE_H };
	SDL_RenderCopy(u->ren, u->res->sprites, &rs, &rd);
}


static void ui_screen_limits(int* x1, int* y1, int* x2, int* y2)
{
	*x1 = *y1 = 0;
	*x2 = *y2 = 11;
}
