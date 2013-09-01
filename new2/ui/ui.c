#include "ui/ui.h"

#include <stdlib.h>
#include <stdio.h>

#include "SDL2/SDL.h"


static bool sdl_init(UI* u);
static void sdl_end(UI* u);


// create the UI object and initialize the user interface
UI* ui_init()
{
	UI* u = malloc(sizeof(UI));

	if(!sdl_init(u))
		return NULL;

	u->active = true;
	u->res = resources_init();
	if(!u->res)
		return NULL;
	
	return u;
}


// free the UI object
void ui_free(UI** u)
{
	resources_free(&(*u)->res);
	sdl_end(*u);
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
		}
	}
}


// present the image to the user
void ui_render(UI* u)
{
}


/********************
 *                  *
 * STATIC FUNCTIONS *
 *                  *
 ********************/
static bool sdl_init(UI* u)
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


static void sdl_end(UI* u)
{
	SDL_DestroyRenderer(u->ren);
	SDL_DestroyWindow(u->win);
	SDL_Quit();
}
