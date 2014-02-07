#ifndef UI_BACKGROUND_H
#define UI_BACKGROUND_H

#include "SDL2/SDL.h"

typedef struct Background {
    SDL_Window* win;
    SDL_Renderer* ren;
    SDL_Texture* tx;
} Background;

void bg_init();
void bg_free();

void bg_recreate();
void bg_render();
void bg_redraw();

#endif


// vim: ts=4:sw=4:sts=4:expandtab
