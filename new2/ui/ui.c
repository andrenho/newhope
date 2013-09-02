#include "ui/ui.h"

#include <stdlib.h>
#include <stdio.h>

#include "SDL2/SDL_timer.h"

#include "engine/world.h"


static bool ui_sdl_init(UI* u);
static void ui_sdl_end(UI* u);
static void ui_screen_limits(UI* u, int* x1, int* y1, int* x2, int* y2);
static void ui_draw_terrain(UI* u, World* w, int x, int y, Terrain t);
static void ui_draw_person(UI* u, World* w, Person* p);
static void ui_center_hero(UI* u, World* w);
static void ui_create_background(UI* u);

static void ui_keyboard_event(UI* u, World* w, SDL_KeyboardEvent k);


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
	u->last_frame = SDL_GetTicks();

	ui_create_background(u);
	
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
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			ui_keyboard_event(u, w, e.key);
			break;
		}
	}
}


// present the image to the user
void ui_render(UI* u, World* w)
{
	ui_center_hero(u, w);

	int x1, y1, x2, y2;
	ui_screen_limits(u, &x1, &y1, &x2, &y2);

	// draw things
	/*
	for(int x=x1; x<=x2; x++) {
		for(int y=y1; y<=y2; y++) {
			Object obj;
			Terrain t = world_xy(w, x, y, &obj);
			ui_draw_terrain(u, w, x, y, t);
		}
	}*/
	SDL_Rect r1 = { 0, 0, 100, 100 };
	SDL_RenderCopy(u->ren, u->bg, &r1, &r1);

	// draw people
	FOREACH(w->people, Person*, p) {
		if(p->x >= (x1-1) && p->x <= x2 && p->y >= (y1-1) && p->y <= y2) {
			ui_draw_person(u, w, p);
		}
	}

	SDL_RenderPresent(u->ren);
}


void ui_wait_next_frame(UI* u)
{
	uint32_t next = u->last_frame + (1000/FPS);
	uint32_t ticks = SDL_GetTicks();
	if(next >= ticks) {
		SDL_Delay(next - ticks);
	} else {
		if(ticks - next > 10) {
			SDL_Log("frame lost: %d ms", ticks - next);
		}
	}
	u->last_frame = SDL_GetTicks();
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


static void ui_screen_limits(UI* u, int* x1, int* y1, int* x2, int* y2)
{
	int w, h;
	SDL_GetWindowSize(u->win, &w, &h);

	*x1 = u->rx / TILE_W - TILE_W;
	*y1 = u->ry / TILE_H - TILE_H;
	*x2 = (u->rx + w) / TILE_W;
	*y2 = (u->ry + h) / TILE_H;
}


static void ui_center_hero(UI* u, World* w)
{
	int _w, _h;
	SDL_GetWindowSize(u->win, &_w, &_h);
	u->rx = w->hero->x * TILE_W - (_w / 2);
	u->ry = w->hero->y * TILE_H - (_h / 2);
}


static void ui_draw_terrain(UI* u, World* w, int x, int y, Terrain t)
{
	SDL_Rect rs = resources_terrain_rect(u->res, t);
	SDL_Rect rd = { .x = (x*TILE_W) - u->rx, .y = (y*TILE_H) - u->ry,
	                .w = TILE_W, .h = TILE_H };
	SDL_RenderCopy(u->ren, u->res->sprites, &rs, &rd);
}


static void ui_draw_person(UI* u, World* w, Person* p)
{
	// body
	SDL_Rect rs = { .x = 0, .y = 16, .w = 16, .h = 16 };
	SDL_Rect rd = { .x = (p->x * TILE_W) - u->rx, .y = (p->y * TILE_H) - u->ry,
			.w = TILE_W, .h = TILE_H };
	SDL_RenderCopy(u->ren, u->res->sprites, &rs, &rd);

	// direction
	SDL_Rect rs2 = { .x = 16, .y = 16, .w = 16, .h = 16 };
	SDL_Rect rd2 = { .x = (p->x * TILE_W) - u->rx, .y = (p->y * TILE_H) - u->ry,
			.w = TILE_W, .h = TILE_H };
	SDL_RenderCopyEx(u->ren, u->res->sprites, &rs2, &rd2, p->direction, NULL, 
			SDL_FLIP_NONE);
}


static void ui_create_background(UI* u)
{
	int w, h;
	SDL_GetWindowSize(u->win, &w, &h);

	u->ren_bg = SDL_CreateRenderer(u->win, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE);
	u->bg = SDL_CreateTexture(u->ren, SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_TARGET, w, h);
	SDL_SetRenderTarget(u->ren_bg, u->bg);
	SDL_SetRenderDrawColor(u->ren_bg, 200, 255, 255, 255);
	SDL_RenderClear(u->ren_bg);
	SDL_SetRenderDrawColor(u->ren_bg, 255, 255, 255, 0);
	SDL_RenderDrawLine(u->ren_bg, 10, 10, 100, 100);
}


static void ui_keyboard_event(UI* u, World* w, SDL_KeyboardEvent k)
{
	if(k.repeat) {
		return;
	}

	// check for moving keys
	const uint8_t* s = SDL_GetKeyboardState(NULL);
	if(s[SDL_SCANCODE_UP] && s[SDL_SCANCODE_LEFT]) {
		person_start_running(w->hero, 315);
	} else if(s[SDL_SCANCODE_UP] && s[SDL_SCANCODE_RIGHT]) {
		person_start_running(w->hero, 45);
	} else if(s[SDL_SCANCODE_DOWN] && s[SDL_SCANCODE_LEFT]) {
		person_start_running(w->hero, 225);
	} else if(s[SDL_SCANCODE_DOWN] && s[SDL_SCANCODE_RIGHT]) {
		person_start_running(w->hero, 135);
	} else if(s[SDL_SCANCODE_UP]) {
		person_start_running(w->hero, 0);
	} else if(s[SDL_SCANCODE_DOWN]) {
		person_start_running(w->hero, 180);
	} else if(s[SDL_SCANCODE_LEFT]) {
		person_start_running(w->hero, 270);
	} else if(s[SDL_SCANCODE_RIGHT]) {
		person_start_running(w->hero, 90);
	} else {
		person_stop_running(w->hero);
	}
}
