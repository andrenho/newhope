#ifndef UI_RESOURCES_H
#define UI_RESOURCES_H

#include <stdint.h>
#include <stdbool.h>

#include "SDL2/SDL.h"
#include "interface.h"
#include "block.h"

SDL_Texture* sprites;

bool resources_init(SDL_Renderer* ren);
void resources_free();

SDL_Rect resources_terrain_rect(BLOCK t);
void resources_person_rect(Person* p, SDL_Rect* person, SDL_Rect* arrow);
void resources_car_rect(Car* c, SDL_Rect* car);

#endif
