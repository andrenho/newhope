#include "minimap.h"

#include <stdlib.h>

#include "luah.h"

Minimap* minimap_new(lua_State* L, SDL_Renderer* ren, int w, int h)
{
	// TODO - separate this function in smaller functions
	
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

	// draw tiles
	for(int x=0; x<w; x++) for(int y=0; y<h; y++) {
		// find tile
		LUA_PUSH_METHOD(L, "tiles");
		lua_pushinteger(L, (-limit_x1+limit_x2) / w * (x-w/2));
		lua_pushinteger(L, (-limit_y1+limit_y2) / h * (y-h/2));
		luah_call(L, 3, 1);
		lua_rawgeti(L, -1, 1);
		/*
		printf("%d %d\n", (-limit_x1+limit_x2) / w * (x-w/2), (-limit_y1+limit_y2) / h * (y-h/2)); 
		if(x == 125 && y == 125) { 
			luah_stack_dump(L); 
		}*/
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

	int prop_w = limit_x1 / (limit_x2 - limit_x1),
	    prop_h = limit_y1 / (limit_y2 - limit_y1);

	// draw rivers
	SDL_Renderer* sr = SDL_CreateSoftwareRenderer(sf);
	SDL_SetRenderDrawColor(sr, 54, 54, 97, SDL_ALPHA_OPAQUE);
	LUA_PUSH_MEMBER(L, "mapgen");
	LUA_PUSH_MEMBER(L, "rivers");
	int nrivers = lua_objlen(L, -1);
	for(int i=0; i<nrivers; i++) {
		// draw river
		lua_rawgeti(L, -1, i+1);
		int npts = lua_objlen(L, -1);
		SDL_Point* points = calloc(sizeof(SDL_Point), npts);
		for(int j=0; j<npts; j++) {
			lua_rawgeti(L, -1, j+1);
			double x, y;
			LUA_FIELD(L, x, "x", number);
			LUA_FIELD(L, y, "y", number);
			points[j].x = (x / (limit_x2 - limit_x1) - prop_w) * w + w/2;
			points[j].y = (y / (limit_y2 - limit_y1) - prop_h) * h + h/2;
			lua_pop(L, 1);
		}
		SDL_RenderDrawLines(sr, points, npts);
		free(points);
		lua_pop(L, 1);
	}
	lua_pop(L, 2);

	// draw cities
	LUA_PUSH_MEMBER(L, "cities");
	SDL_SetRenderDrawColor(sr, 149, 69, 53, SDL_ALPHA_OPAQUE);
	int ncities = lua_objlen(L, -1);
	for(int i=0; i<ncities; i++) {
		// get data
		lua_rawgeti(L, -1, i+1);
		double x, y;
		LUA_FIELD(L, x, "x", number);
		LUA_FIELD(L, y, "y", number);

		// find points
		int px = (x / (limit_x2 - limit_x1) - prop_w) * w + w/2;
		int py = (y / (limit_y2 - limit_y1) - prop_h) * h + h/2;
		SDL_Rect r1 = { px-2, py-2, 5, 5 };
		SDL_Rect r2 = { px-4, py-4, 9, 9 };

		// draw rectangles
		SDL_RenderFillRect(sr, &r1);
		SDL_RenderDrawRect(sr, &r2);

		lua_pop(L, 1);
	}
	lua_pop(L, 1);

	SDL_RenderPresent(sr);
	SDL_DestroyRenderer(sr);

	mm->tx = SDL_CreateTextureFromSurface(ren, sf);
	SDL_FreeSurface(sf);
	return mm;
}


void minimap_draw_player(Minimap* mm, lua_State* L, SDL_Renderer* ren, int ww, int wh)
{
	// get map limits
	LUA_PUSH_WORLD(L);
	LUA_PUSH_METHOD(L, "limits");
	luah_call(L, 1, 4);
	int limit_x1 = lua_tointeger(L, -4),
	    limit_y1 = lua_tointeger(L, -3),
	    limit_x2 = lua_tointeger(L, -2),
	    limit_y2 = lua_tointeger(L, -1);
	lua_pop(L, 4);

	int prop_w = limit_x1 / (limit_x2 - limit_x1),
	    prop_h = limit_y1 / (limit_y2 - limit_y1);

	// draw player
	LUA_PUSH_MEMBER(L, "player");
	LUA_PUSH_METHOD(L, "pos");
	luah_call(L, 1, 1);
	double x, y;
	LUA_FIELD(L, x, "x", number);
	LUA_FIELD(L, y, "y", number);
	int px = (x / (limit_x2 - limit_x1) - prop_w) * mm->w + (ww/2);
	int py = (y / (limit_y2 - limit_y1) - prop_h) * mm->h + (wh/2);
	SDL_Rect r1 = { px-2, py-2, 5, 5 };
	SDL_Rect r2 = { px-3, py-3, 7, 7 };
	SDL_SetRenderDrawColor(ren, 255, 255, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(ren, &r1);
	SDL_SetRenderDrawColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(ren, &r2);
}


void minimap_destroy(Minimap** mm)
{
	// TODO
}
