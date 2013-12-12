#include "render.h"

#include <math.h>
#include "SDL2/SDL.h"
#include "lauxlib.h"

#include "interface.h"

extern SDL_Window* win;
extern SDL_Renderer* ren;
static float rx = 0.0, ry = 0.0, scale = 1.5;

void init_render()
{
	SDL_RenderSetScale(ren, scale, scale);
}


int cb_render(lua_State* L)
{
	// TODO
	return 0;
}


int cb_render_physics(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TTABLE);

	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	SDL_RenderClear(ren);

	int scr_w, scr_h;
	SDL_GetWindowSize(win, &scr_w, &scr_h);

	int n = luaL_len(L, 1);
	for(int i=1; i<=n; i++) {
		lua_rawgeti(L, 1, i);

		double x, y, w, h, angle;
		LUA_FIELD(x, "x", number);
		LUA_FIELD(y, "y", number);
		LUA_FIELD(w, "w", number);
		LUA_FIELD(h, "h", number);
		LUA_FIELD(angle, "angle", number);

		// get angle
		double x1 = -w/2 * 8;
		double y1 = -h/2 * 8;
		double x2 =  w/2 * 8;
		double y2 =  h/2 * 8;
		double px1 = x1 * cos(angle) - y1 * sin(angle) + (x*8);
		double py1 = x1 * sin(angle) + y1 * cos(angle) + (y*8);
		double px2 = x1 * cos(angle) - y2 * sin(angle) + (x*8);
		double py2 = x1 * sin(angle) + y2 * cos(angle) + (y*8);
		double px3 = x2 * cos(angle) - y2 * sin(angle) + (x*8);
		double py3 = x2 * sin(angle) + y2 * cos(angle) + (y*8);
		double px4 = x2 * cos(angle) - y1 * sin(angle) + (x*8);
		double py4 = x2 * sin(angle) + y1 * cos(angle) + (y*8);

		// get deslocation
		double desloc_x = (scr_w/2/scale) + rx;
		double desloc_y = (scr_h/2/scale) + ry;

		// draw points
		const SDL_Point pts[] = {
			{ px1+desloc_x, py1+desloc_y },
			{ px2+desloc_x, py2+desloc_y },
			{ px3+desloc_x, py3+desloc_y },
			{ px4+desloc_x, py4+desloc_y },
			{ px1+desloc_x, py1+desloc_y },
		};
		SDL_SetRenderDrawColor(ren, 0, 128, 0, 255);
		SDL_RenderDrawLines(ren, pts, 5);
	}

	SDL_RenderPresent(ren);
	return 0;
}


int cb_window_tiles(lua_State* L)
{
	int w, h;
	SDL_GetWindowSize(win, &w, &h);

	lua_pushinteger(L, rx-(w/2 / scale));
	lua_pushinteger(L, ry-(h/2 / scale));
	lua_pushinteger(L, rx+(w/2 / scale));
	lua_pushinteger(L, ry+(h/2 / scale));

	return 4;
}
