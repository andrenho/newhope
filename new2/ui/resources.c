#include "ui/resources.h"

#include <stdlib.h>
#include <stdbool.h>

#include "SDL2/SDL_image.h"

static bool resources_load(Resources* r, SDL_Renderer* ren);


Resources* resources_init(SDL_Renderer* ren)
{
	Resources* r = malloc(sizeof(Resources));
	IMG_Init(IMG_INIT_PNG);
	if(!resources_load(r, ren))
		return NULL;
	return r;
}


void resources_free(Resources** r)
{
	SDL_DestroyTexture((*r)->sprites);
	IMG_Quit();
	free(*r);
	*r = NULL;
}


static struct RES_Image {
	enum Terrain terrain;
	SDL_Rect r;
} res_image[] = {
	{ OUT_OF_BOUNDS, 	{ 16,  0, 16, 16 } },
	{ GRASS, 		{  0,  0, 16, 16 } },
};

SDL_Rect resources_terrain_rect(Resources* r, Terrain t)
{
	for(int i=0; i<(sizeof(res_image)/sizeof(struct RES_Image)); i++) {
		if(res_image[i].terrain == t)
			return res_image[i].r;
	}
	return (SDL_Rect) { 0, 0, 0, 0 };
}


/********************
 *                  *
 * STATIC FUNCTIONS *
 *                  *
 ********************/


static bool resources_load(Resources* r, SDL_Renderer* ren)
{
	SDL_Surface* sf = IMG_Load("data/sprites.png");
	if(!sf) {
		SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "error load image: %s",
				SDL_GetError());
		return false;
	}
	r->sprites = SDL_CreateTextureFromSurface(ren, sf);

	SDL_FreeSurface(sf);
	return true;
}
