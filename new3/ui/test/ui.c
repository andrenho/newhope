#include "ui.h"

#include <stdlib.h>
#include <stdio.h>

#include "SDL2/SDL_timer.h"
#include "interface.h"

UI ui;

static bool ui_sdl_init();
static void ui_sdl_end();
static void ui_center_hero();
static void ui_keyboard_event(SDL_KeyboardEvent k);

// create the UI object and initialize the user interface
bool ui_init()
{
	if(!ui_sdl_init())
		return false;

	ui.active = true;
	ui.rx = 0;
	ui.ry = 0;
	ui.last_frame = SDL_GetTicks();

	if(!resources_init(ui.ren))
		return false;
	bg_init(&ui);

	return true;
}


// free the UI object
void ui_finish()
{
	bg_free();
	resources_free();
	ui_sdl_end();
}


// return if the UI is still active
bool ui_active()
{
	return ui.active;
}


// parse user input and send the events to the engine
void ui_do_events()
{
	SDL_Event e;
	while(SDL_PollEvent(&e)) {
		switch(e.type) {
		case SDL_QUIT:
			ui.active = false;
			break;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			ui_keyboard_event(e.key);
			break;
		case SDL_WINDOWEVENT:
			if(e.window.event == SDL_WINDOWEVENT_RESIZED) {
				bg_recreate();
			}
			break;
		}
	}
}


// present the image to the user
void ui_render()
{
	Uint32 t = SDL_GetTicks();

	ui_center_hero();

	int x1, y1, x2, y2;
	ui_screen_limits(&x1, &y1, &x2, &y2);

	// draw background
	bg_render();

	SDL_RenderPresent(ui.ren);

	if(SDL_GetTicks() - t > (1000/FPS)+5)
		printf("frame lost: %zu ms\n", SDL_GetTicks() - t);
}


void ui_wait_next_frame()
{
	uint32_t next = ui.last_frame + (1000/FPS);
	uint32_t ticks = SDL_GetTicks();
	if(next >= ticks) {
		SDL_Delay(next - ticks);
	} else {
		if(ticks - next > 5) {
			SDL_Log("frame lost: %d ms", ticks - next);
		}
	}
	ui.last_frame = SDL_GetTicks();
}


void ui_screen_limits(int* x1, int* y1, int* x2, int* y2)
{
	int w, h;
	SDL_GetWindowSize(ui.win, &w, &h);

	*x1 = ui.rx / TILE_W - TILE_W;
	*y1 = ui.ry / TILE_H - TILE_H;
	*x2 = (ui.rx + w) / TILE_W;
	*y2 = (ui.ry + h) / TILE_H;
}


/********************
 *                  *
 * STATIC FUNCTIONS *
 *                  *
 ********************/


static bool ui_sdl_init()
{
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "error: unable to initialize SDL: %s\n", 
				SDL_GetError());
		return false;
	}

	if((ui.win = SDL_CreateWindow("New Hope " VERSION, SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED, 800, 600, 
				SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL)) == NULL) {
		SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "error initializing "
				"window: %s\n", SDL_GetError());
		return false;
	}
	ui.ren = SDL_CreateRenderer(ui.win, -1, 
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	return true;
}


static void ui_sdl_end()
{
	SDL_DestroyRenderer(ui.ren);
	SDL_DestroyWindow(ui.win);
	SDL_Quit();
}


static void ui_center_hero()
{
	int _w, _h;
	SDL_GetWindowSize(ui.win, &_w, &_h);
	if(_h % 2)
		_h++;

	double hero_x, hero_y;
	if_hero_position(&hero_x, &hero_y);
	ui.rx = (hero_x * TILE_W) - (_w / 2.0);
	ui.ry = (hero_y * TILE_H) - (_h / 2.0);

	bg_redraw();
}


static void ui_keyboard_event(SDL_KeyboardEvent k)
{
	/*
	if(k.repeat) {
		return;
	}

	// check for moving keys
	const uint8_t* s = SDL_GetKeyboardState(NULL);
	if(s[SDL_SCANCODE_UP] && s[SDL_SCANCODE_LEFT]) {
		person_start_running(ui.w->hero, 315);
	} else if(s[SDL_SCANCODE_UP] && s[SDL_SCANCODE_RIGHT]) {
		person_start_running(ui.w->hero, 45);
	} else if(s[SDL_SCANCODE_DOWN] && s[SDL_SCANCODE_LEFT]) {
		person_start_running(ui.w->hero, 225);
	} else if(s[SDL_SCANCODE_DOWN] && s[SDL_SCANCODE_RIGHT]) {
		person_start_running(ui.w->hero, 135);
	} else if(s[SDL_SCANCODE_UP]) {
		person_start_running(ui.w->hero, 0);
	} else if(s[SDL_SCANCODE_DOWN]) {
		person_start_running(ui.w->hero, 180);
	} else if(s[SDL_SCANCODE_LEFT]) {
		person_start_running(ui.w->hero, 270);
	} else if(s[SDL_SCANCODE_RIGHT]) {
		person_start_running(ui.w->hero, 90);
	} else {
		person_stop_running(ui.w->hero);
	}
	*/
}
