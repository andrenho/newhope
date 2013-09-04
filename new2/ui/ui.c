#include "ui/ui.h"

#include <stdlib.h>
#include <stdio.h>

#include "SDL2/SDL_timer.h"

#include "engine/world.h"


static bool ui_sdl_init(UI* u);
static void ui_sdl_end(UI* u);
static void ui_center_hero(UI* u);

static void ui_keyboard_event(UI* u, SDL_KeyboardEvent k);


// create the UI object and initialize the user interface
UI* ui_init(World* w)
{
	UI* u = malloc(sizeof(UI));
	u->w = w;

	if(!ui_sdl_init(u))
		return NULL;

	u->active = true;
	u->res = resources_init(u->ren);
	if(!u->res)
		return NULL;
	u->rx = 0;
	u->ry = 0;
	u->last_frame = SDL_GetTicks();
	u->bg = bg_init(u);
	
	return u;
}


// free the UI object
void ui_free(UI** u)
{
	bg_free(&(*u)->bg);
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
void ui_do_events(UI* u)
{
	SDL_Event e;
	while(SDL_PollEvent(&e)) {
		switch(e.type) {
		case SDL_QUIT:
			u->active = false;
			break;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			ui_keyboard_event(u, e.key);
			break;
		case SDL_WINDOWEVENT:
			if(e.window.event == SDL_WINDOWEVENT_RESIZED) {
				bg_recreate(u->bg);
			}
			break;
		}
	}
}


// present the image to the user
void ui_render(UI* u)
{
	Uint32 t = SDL_GetTicks();

	ui_center_hero(u);

	int x1, y1, x2, y2;
	ui_screen_limits(u, &x1, &y1, &x2, &y2);

	// draw background
	bg_render(u->bg);

	SDL_RenderPresent(u->ren);

	if(SDL_GetTicks() - t > (1000/FPS)+5)
		printf("frame lost: %zu ms\n", SDL_GetTicks() - t);
}


void ui_wait_next_frame(UI* u)
{
	uint32_t next = u->last_frame + (1000/FPS);
	uint32_t ticks = SDL_GetTicks();
	if(next >= ticks) {
		SDL_Delay(next - ticks);
	} else {
		if(ticks - next > 5) {
			SDL_Log("frame lost: %d ms", ticks - next);
		}
	}
	u->last_frame = SDL_GetTicks();
}


void ui_screen_limits(UI* u, int* x1, int* y1, int* x2, int* y2)
{
	int w, h;
	SDL_GetWindowSize(u->win, &w, &h);

	*x1 = u->rx / TILE_W - TILE_W;
	*y1 = u->ry / TILE_H - TILE_H;
	*x2 = (u->rx + w) / TILE_W;
	*y2 = (u->ry + h) / TILE_H;
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

	if((u->win = SDL_CreateWindow("New Hope " VERSION, SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED, 800, 600, 
				SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL)) == NULL) {
		SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "error initializing "
				"window: %s\n", SDL_GetError());
		return false;
	}
	u->ren = SDL_CreateRenderer(u->win, -1, 
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	return true;
}


static void ui_sdl_end(UI* u)
{
	SDL_DestroyRenderer(u->ren);
	SDL_DestroyWindow(u->win);
	SDL_Quit();
}


static void ui_center_hero(UI* u)
{
	int _w, _h;
	SDL_GetWindowSize(u->win, &_w, &_h);
	if(_h % 2)
		_h++;
	u->rx = (u->w->hero->x * TILE_W) - (_w / 2.0);
	u->ry = (u->w->hero->y * TILE_H) - (_h / 2.0);

	bg_redraw(u->bg);
}


static void ui_keyboard_event(UI* u, SDL_KeyboardEvent k)
{
	if(k.repeat) {
		return;
	}

	// check for moving keys
	const uint8_t* s = SDL_GetKeyboardState(NULL);
	if(s[SDL_SCANCODE_UP] && s[SDL_SCANCODE_LEFT]) {
		person_start_running(u->w->hero, 315);
	} else if(s[SDL_SCANCODE_UP] && s[SDL_SCANCODE_RIGHT]) {
		person_start_running(u->w->hero, 45);
	} else if(s[SDL_SCANCODE_DOWN] && s[SDL_SCANCODE_LEFT]) {
		person_start_running(u->w->hero, 225);
	} else if(s[SDL_SCANCODE_DOWN] && s[SDL_SCANCODE_RIGHT]) {
		person_start_running(u->w->hero, 135);
	} else if(s[SDL_SCANCODE_UP]) {
		person_start_running(u->w->hero, 0);
	} else if(s[SDL_SCANCODE_DOWN]) {
		person_start_running(u->w->hero, 180);
	} else if(s[SDL_SCANCODE_LEFT]) {
		person_start_running(u->w->hero, 270);
	} else if(s[SDL_SCANCODE_RIGHT]) {
		person_start_running(u->w->hero, 90);
	} else {
		person_stop_running(u->w->hero);
	}
}
