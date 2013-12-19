#include"wireframe.h"

#include <stdbool.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <lauxlib.h>

#include "luah.h"
#include "physics.h"

// TODO
// TODO - restrict all to visible area
// TODO

const double Z = 15;
int rx = 50, ry = 50;

static void draw_circle(cpBody *body, cpShape *shape, void* data)
{
	SDL_Renderer* ren = (SDL_Renderer*)data;
	cpVect pos = cpBodyGetPos(body);
	cpFloat r = cpCircleShapeGetRadius(shape);
	aacircleRGBA(ren, pos.x*Z+rx, pos.y*Z+ry, r*Z, 0, 80, 0, 255);
}

static void draw_static_shape(cpBody *body, cpShape *shape, void* data)
{
	SDL_Renderer* ren = (SDL_Renderer*)data;
	cpBB bb = cpShapeGetBB(shape);
	rectangleRGBA(ren, bb.l*Z+rx, bb.b*Z+ry, bb.r*Z+rx, bb.t*Z+ry, 168, 0, 0, 255);
}

void wireframe_render(lua_State* L, SDL_Renderer* ren)
{
	// draw static bodies
	cpBodyEachShape(space->staticBody, draw_static_shape, ren);

	// draw other objects
	int n = luaL_len(L, 2);
	for(int i=1; i<=n; i++)
	{
		lua_rawgeti(L, 2, i);

		// TODO - check if it's car
		bool is_car = false;

		if(is_car) {
			abort();
		} else {
			cpBody* body;
			LUA_FIELD(L, body, "body", userdata);
			cpBodyEachShape(body, draw_circle, ren);
		}
	}
}
