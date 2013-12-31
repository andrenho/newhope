#include "minimap.h"

#include <stdlib.h>

#include "luah.h"

Minimap* minimap_new(lua_State* L, SDL_Renderer* ren, int w, int h)
{
	Minimap* mm = malloc(sizeof(Minimap));
	mm->w = w;
	mm->h = h;
	
	// get map limits
	LUA_PUSH_WORLD(L);
	LUA_PUSH_METHOD(L, "limits");
	luah_call(L, 1, 4);
	int limit_x1 = lua_tointeger(L, -4),
	    limit_y1 = lua_tointeger(L, -3),
	    limit_x2 = lua_tointeger(L, -2),
	    limit_y2 = lua_tointeger(L, -1);
	lua_pop(L, 4);

	// create texture
	SDL_Surface* sf = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
	for(int x=0; x<w; x++) for(int y=0; y<h; y++) {
		// find tile
		LUA_PUSH_METHOD(L, "tiles");
		lua_pushinteger(L, (-limit_x1+limit_x2) / w * (x-w/2));
		lua_pushinteger(L, (-limit_y1+limit_y2) / h * (y-h/2));
		luah_call(L, 3, 1);
		lua_rawgeti(L, -1, 1);
		// get tile colors
		lua_pushstring(L, "color");
		lua_gettable(L, -2);
		lua_rawgeti(L, -1, 1),
		lua_rawgeti(L, -2, 2),
		lua_rawgeti(L, -3, 3);
		int r = lua_tointeger(L, -3),
		    g = lua_tointeger(L, -2),
		    b = lua_tointeger(L, -1);
		// set pixel color
		Uint8* pixel = (Uint8*)sf->pixels;
		pixel += (y * sf->pitch) + (x * sizeof(Uint32));
		*((Uint32*)pixel) = (r<<16) | (g<<8) | (b);

		lua_pop(L, 6);
	}
	mm->tx = SDL_CreateTextureFromSurface(ren, sf);
	SDL_FreeSurface(sf);

	return mm;
}


void minimap_destroy(Minimap** mm)
{
	// TODO
}
