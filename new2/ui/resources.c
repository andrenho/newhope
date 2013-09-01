#include "ui/resources.h"

#include <stdlib.h>
#include <stdbool.h>

#include "SDL2/SDL_image.h"

static bool resources_load(Resources* r, SDL_Renderer* ren);
static int resources_count();


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
	for(int i=0; i<resources_count(); i++) {
		SDL_DestroyTexture((*r)->texture[i]);
	}
	free((*r)->texture);
	IMG_Quit();
	free(*r);
	*r = NULL;
}


SDL_Texture* resources_terrain_texture(Resources* r, Terrain t)
{
	return r->texture[R_GRASS];
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

static bool resources_load(Resources* r, SDL_Renderer* ren)
{
	SDL_Surface* sf = IMG_Load("data/sprites.png");
	if(!sf) {
		SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "error load image: %s",
				SDL_GetError());
		return false;
	}

	r->texture = calloc(resources_count(), sizeof(SDL_Texture*));
	
	for(int i=0; i<resources_count(); i++) {
		struct RES_Image ri = res_image[i];
		SDL_Surface* s = SDL_CreateRGBSurface(0, ri.w, ri.h, 
				sf->format->BitsPerPixel, sf->format->Rmask,
				sf->format->Gmask, sf->format->Bmask,
				sf->format->Amask);
		SDL_Rect rect = { .x = ri.x, .y = ri.y, .w = ri.w, .h = ri.h };
		SDL_BlitSurface(sf, &rect, s, NULL);
		r->texture[i] = SDL_CreateTextureFromSurface(ren, s);
		SDL_FreeSurface(s);
	}

	SDL_FreeSurface(sf);
	return true;
}


static int resources_count()
{
	return sizeof(res_image)/sizeof(struct RES_Image);
}
