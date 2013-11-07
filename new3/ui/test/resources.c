#include "resources.h"

#include <stdlib.h>

#include "SDL2/SDL_image.h"

static bool resources_load(SDL_Renderer* ren);

bool resources_init(SDL_Renderer* ren)
{
	IMG_Init(IMG_INIT_PNG);
	if(!resources_load(ren))
		return false;
	return true;
}


void resources_free()
{
	SDL_DestroyTexture(sprites);
	IMG_Quit();
}


SDL_Rect resources_terrain_rect(BLOCK t)
{
	static struct TerrainImage {
		BLOCK terrain;
		SDL_Rect r;
	} res_image[] = {
		{ NOTHING,	{ 16,  0, 16, 16 } },
		{ GRASS, 	{  0,  0, 16, 16 } },
		{ WATER,        { 32,  0, 16, 16 } },
	};

	for(int i=0; i<(sizeof(res_image)/sizeof(struct TerrainImage)); i++) {
		if(res_image[i].terrain == t) {
			return res_image[i].r;
		}
	}
	return (SDL_Rect) { 0, 0, 0, 0 };
}

/*
SDL_Rect resources_obj_rect(Resources* r, Object obj)
{
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

	for(int i=0; i<(sizeof(res_obj)/sizeof(struct ObjectImage)); i++) {
		if(res_obj[i].type == obj.type && res_obj[i].code == obj.code) {
			return res_obj[i].r;
		}
	}
	return (SDL_Rect) { 0, 0, 0, 0 };
}


void resources_person_rect(Resources* r, Person* p, SDL_Rect* person, 
		SDL_Rect* arrow)
{
	static struct PersonImage {
		int image;
		SDL_Rect rp;
		SDL_Rect ra;
	} res_p[] = {
		{ 0, {  0, 16, 16, 16}, { 16, 16, 16, 16 } },
		{ 1, { 32, 16, 16, 16}, { 48, 16, 16, 16 } },
		{ 2, { 64, 16, 16, 16}, { 80, 16, 16, 16 } },
	};
	for(int i=0; i<(sizeof(res_p)/sizeof(struct PersonImage)); i++) {
		if(res_p[i].image == p->image) {
			*person = res_p[i].rp;
			*arrow = res_p[i].ra;
			return;
		}
	}
	*person = (SDL_Rect) { 0, 0, 0, 0 };
	*arrow  = (SDL_Rect) { 0, 0, 0, 0 };
}
*/

/********************
 *                  *
 * STATIC FUNCTIONS *
 *                  *
 ********************/


static bool resources_load(SDL_Renderer* ren)
{
	SDL_Surface* sf = IMG_Load("sprites.png");
	if(!sf) {
		SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "error load image: %s",
				SDL_GetError());
		return false;
	}
	sprites = SDL_CreateTextureFromSurface(ren, sf);

	SDL_FreeSurface(sf);
	return true;
}
