#ifndef MINIMAP_H
#define MINIMAP_H

#include <SDL2/SDL.h>
#include <lua.h>

typedef struct Minimap {
    int w, h;
    SDL_Texture* tx;
} Minimap;

Minimap* minimap_new(lua_State* L, SDL_Renderer* ren, int w, int h);
void minimap_draw_player(Minimap* mm, lua_State* L, SDL_Renderer* ren, int ww, int wh);
void minimap_destroy(Minimap** mm);

#endif


// vim: ts=4:sw=4:sts=4:expandtab
