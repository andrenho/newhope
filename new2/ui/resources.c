#include "ui/resources.h"

#include <stdlib.h>
#include <stdbool.h>

#include "SDL2/SDL_image.h"

static bool resources_load(Resources* r);


Resources* resources_init()
{
	Resources* r = malloc(sizeof(Resources));
	IMG_Init(IMG_INIT_PNG);
	if(!resources_load(r))
		return NULL;
	return r;
}


void resources_free(Resources** r)
{
	IMG_Quit();
	free(*r);
	*r = NULL;
}


/********************
 *                  *
 * STATIC FUNCTIONS *
 *                  *
 ********************/


static struct RES_Image {
	enum Resource res_n;
	int x, y, w, h;
} res_image[] = {
	{ R_GRASS, 0, 0, 16, 16 },
};

static bool resources_load(Resources* r)
{
	SDL_Surface* sf = IMG_Load("data/sprites.png");
	if(!sf) {
		SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "error load image: %s",
				SDL_GetError());
		return false;
	}

	r->texture = calloc(sizeof(res_image) / sizeof(struct RES_Image), 
			sizeof(SDL_Texture*));
	
	int i;
	for(i=0; i<(sizeof(res_image)/sizeof(struct RES_Image)); i++) {
		struct RES_Image ri = res_image[i];
		//SDL_Surface* s = SDL_CreateRGBSurface(0, ri->w, ri->h,
	}

	SDL_FreeSurface(sf);
	return true;
}
