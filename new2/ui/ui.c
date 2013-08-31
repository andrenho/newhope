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
	return u;
}


// free the UI object
void ui_free(UI** u)
{
	sdl_end(*u);
	free(*u);
	*u = NULL;
}


// return if the UI is still active
bool ui_active()
{
	return true;
}


// parse user input and send the events to the engine
void ui_do_events()
{
}


// present the image to the user
void ui_render()
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


	return true;
}


static void sdl_end(UI* u)
{
}
