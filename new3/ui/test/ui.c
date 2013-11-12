#include "ui.h"

#ifndef M_PI
#  define M_PI 3.14159265358979323846
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
	//Uint32 t = SDL_GetTicks();
	if(if_in_error)
	{
		// in case of error, clear the screen and returns
		SDL_SetRenderDrawColor(ui.ren, 255, 255, 255, 255);
		SDL_RenderClear(ui.ren);
		SDL_RenderPresent(ui.ren);
		return;
	}

	ui_center_hero();

	int x1, y1, x2, y2;
	ui_screen_limits(&x1, &y1, &x2, &y2);

	// draw background
	bg_render();

	SDL_RenderPresent(ui.ren);

	//if(SDL_GetTicks() - t > (1000/FPS)+5)
	//	printf("frame lost: %zu ms\n", SDL_GetTicks() - t);
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


void ui_show_message(Message* msg)
{
	char **str = NULL;
	int n = if_wrap("If you were growing up in or around the 90s, you probably loved The Mighty Ducks. Sure, it was poorly acted, full of ridiculous plot points and implausible scenarios, but I bet you have fond memories of this hockey team.",
			40, &str);
	int adv;
	TTF_GlyphMetrics(ui.font, 'a', NULL, NULL, NULL, NULL, &adv);
	SDL_SetRenderDrawColor(ui.ren, 0, 0, 0, 255);
	SDL_RenderFillRect(ui.ren, &(SDL_Rect) { 0, 0, (adv*40) + 20, (TTF_FontLineSkip(ui.font)*n) + 20 });
	int y = 10;
	for(int i=0; i<n; i++)
	{
		SDL_Surface* txt = TTF_RenderUTF8_Solid(ui.font, str[i], (SDL_Color){ 255, 255, 255 });
		SDL_Texture* tt = SDL_CreateTextureFromSurface(ui.ren, txt);

		SDL_Rect r = { 10, y, txt->w, txt->h };
		y += TTF_FontLineSkip(ui.font);

		SDL_FreeSurface(txt);

		SDL_RenderCopy(ui.ren, tt, NULL, &r);

		SDL_DestroyTexture(tt);
	}
	SDL_RenderPresent(ui.ren);
}


MessageResponse ui_respond_message(Message* msg)
{
	SDL_Event e;
	while(SDL_WaitEvent(&e)) {
		switch(e.type) {
		case SDL_QUIT:
			ui.active = false;
			return (MessageResponse){ .option = 0 };
		}
	}

	return (MessageResponse){ .option = 0 };
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

	if(TTF_Init() == -1) {
		fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
		return false;
	}
	if((ui.font = TTF_OpenFont("PressStart2P.ttf", 8)) == NULL) {
		fprintf(stderr, "TTF_OpenFont: %s\n", TTF_GetError());
		return false;
	}

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
	if(k.repeat) {
		return;
	}

	// CTRL+R - reload engine
	if(k.type == SDL_KEYDOWN && k.keysym.sym == SDLK_r && 
			k.keysym.mod & KMOD_CTRL) {
		if_init();
		fprintf(stderr, "Engine reloaded.\n");
	}

	// if is in error, only CTRL+R is allowed
	if(if_in_error)
		return;

	// check for moving keys
	const uint8_t* s = SDL_GetKeyboardState(NULL);
	if(s[SDL_SCANCODE_DOWN] && s[SDL_SCANCODE_LEFT]) {
		if_hero_move(2, 3*M_PI/4.0);
	} else if(s[SDL_SCANCODE_DOWN] && s[SDL_SCANCODE_RIGHT]) {
		if_hero_move(2, M_PI/3.0);
	} else if(s[SDL_SCANCODE_UP] && s[SDL_SCANCODE_LEFT]) {
		if_hero_move(2, 5*M_PI/4.0);
	} else if(s[SDL_SCANCODE_UP] && s[SDL_SCANCODE_RIGHT]) {
		if_hero_move(2, 7*M_PI/4.0);
	} else if(s[SDL_SCANCODE_DOWN]) {
		if_hero_move(2, M_PI/2.0);
	} else if(s[SDL_SCANCODE_UP]) {
		if_hero_move(2, 3*M_PI/2.0);
	} else if(s[SDL_SCANCODE_LEFT]) {
		if_hero_move(2, M_PI);
	} else if(s[SDL_SCANCODE_RIGHT]) {
		if_hero_move(2, 2*M_PI);
	} else {
		if_hero_move(0, 0);
	}
}
