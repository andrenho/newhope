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
		LUA_PUSH_METHOD(L, "tiles");
		lua_pushinteger(L, x);
		lua_pushinteger(L, y);
		luah_call(L, 3, 1);
		lua_rawgeti(L, -1, 1);
		lua_pushstring(L, "color");
		lua_gettable(L, -2);
		lua_rawgeti(L, -1, 1),
		lua_rawgeti(L, -2, 2),
		lua_rawgeti(L, -3, 3);
		int r = lua_tointeger(L, -3),
		    g = lua_tointeger(L, -2),
		    b = lua_tointeger(L, -1);
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
