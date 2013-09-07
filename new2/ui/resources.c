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


static struct TerrainImage {
	enum Terrain terrain;
	SDL_Rect r;
} res_image[] = {
	{ OUT_OF_BOUNDS, 	{ 16,  0, 16, 16 } },
	{ GRASS, 		{  0,  0, 16, 16 } },
	{ OCEAN,                { 32,  0, 16, 16 } },
};

SDL_Rect resources_terrain_rect(Resources* r, Terrain t)
{
	for(int i=0; i<(sizeof(res_image)/sizeof(struct TerrainImage)); i++) {
		if(res_image[i].terrain == t) {
			return res_image[i].r;
		}
	}
	return (SDL_Rect) { 0, 0, 0, 0 };
}


static struct ObjectImage {
	enum ObjectType type;
	int code;
	SDL_Rect r;
} res_obj[] = {
	{ WALL, WALL_NW, {  0, 32, 16, 16 } },
	{ WALL, WALL_N,  { 16, 32, 16, 16 } },
	{ WALL, WALL_NE, { 32, 32, 16, 16 } },
	{ WALL, WALL_W,  {  0, 48, 16, 16 } },
	{ WALL, WALL_E,  { 32, 48, 16, 16 } },
	{ WALL, WALL_SW, {  0, 64, 16, 16 } },
	{ WALL, WALL_S,  { 16, 64, 16, 16 } },
	{ WALL, WALL_SE, { 32, 64, 16, 16 } },
	{ FLOOR, 0,      { 16, 48, 16, 16 } },
	{ DOOR, 0,       {  0, 80, 16, 16 } },
	{ DOOR, 1,       { 16, 80, 16, 16 } },
};
SDL_Rect resources_obj_rect(Resources* r, Object obj)
{
	for(int i=0; i<(sizeof(res_obj)/sizeof(struct ObjectImage)); i++) {
		if(res_obj[i].type == obj.type && res_obj[i].code == obj.code) {
			return res_obj[i].r;
		}
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
