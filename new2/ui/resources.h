#ifndef UI_RESOURCES_H
#define UI_RESOURCES_H

#include "SDL2/SDL.h"

#include "engine/terrain.h"
#include "engine/object.h"
#include "engine/person.h"

typedef struct Resources {
	SDL_Texture* sprites;
} Resources;

Resources* resources_init(SDL_Renderer* ren);
void resources_free(Resources** r);
SDL_Rect resources_terrain_rect(Resources* r, Terrain t);
SDL_Rect resources_obj_rect(Resources* r, Object o);
void resources_person_rect(Resources* r, Person* p, SDL_Rect* person, 
		SDL_Rect* arrow);

#endif
