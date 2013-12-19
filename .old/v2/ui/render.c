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


static void render_rectangle(double x, double y, double w, double h, double angle,
		SDL_Color color)
{
	int scr_w, scr_h;
	SDL_GetWindowSize(win, &scr_w, &scr_h);

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
	SDL_SetRenderDrawColor(ren, 0, color.r, color.g, color.b);
	SDL_RenderDrawLines(ren, pts, 5);
}


SDL_Point circle_pixel[10000];
static void render_circle(double x1, double y1, double radius, SDL_Color color)
{
	int scr_w, scr_h;
	SDL_GetWindowSize(win, &scr_w, &scr_h);

	int x0 = (x1*8) + (scr_w/2/scale) + rx;
	int y0 = (y1*8) + (scr_h/2/scale) + ry;

	int x = 4, y = 0;
	int radiusError = 1-x;
	int i = 0;

	while(x >= y) {
		circle_pixel[i++] = (const SDL_Point){ x + x0, y + y0 };
		circle_pixel[i++] = (const SDL_Point){ y + x0, x + y0 };
		circle_pixel[i++] = (const SDL_Point){ -x + x0, y + y0 };
		circle_pixel[i++] = (const SDL_Point){ -y + x0, x + y0 };
		circle_pixel[i++] = (const SDL_Point){ -x + x0, -y + y0 };
		circle_pixel[i++] = (const SDL_Point){ -y + x0, -x + y0 };
		circle_pixel[i++] = (const SDL_Point){ x + x0, -y + y0 };
		circle_pixel[i++] = (const SDL_Point){ y + x0, -x + y0 };
		y++;
		if(radiusError<0)
			radiusError += 2*y+1;
		else {
			x--;
			radiusError += 2*(y-x+1);
		}
	}

	SDL_SetRenderDrawColor(ren, 0, color.r, color.g, color.b);
	SDL_RenderDrawPoints(ren, circle_pixel, i);
}


static void render_physics_objects(lua_State* L)
{
	int n = luaL_len(L, 1);
	for(int i=1; i<=n; i++) {
		lua_rawgeti(L, 1, i);

		// get object type
		LUA_PUSH_METHOD("is_car");
		if_call(1, 1);
		int is_car = lua_toboolean(L, -1);
		lua_pop(L, 1);

		// draw car
		if(is_car) {
			double x, y, w, h, angle;
			LUA_FIELD(x, "x", number);
			LUA_FIELD(y, "y", number);
			LUA_FIELD(angle, "angle", number);
			lua_pushstring(L, "attrib");
			lua_gettable(L, -2);
			LUA_FIELD(w, "w", number);
			LUA_FIELD(h, "h", number);
			render_rectangle(x, y, w, h, angle, (SDL_Color){128,0,0});
		} else { // draw person
			double x, y;
			LUA_FIELD(x, "x", number);
			LUA_FIELD(y, "y", number);
			render_circle(x, y, 1, (SDL_Color){0,0,0});
		}
	}
}


static void render_physics_static(lua_State* L)
{
	int x1 = luaL_checknumber(L, 2),
	    y1 = luaL_checknumber(L, 3),
	    x2 = luaL_checknumber(L, 4),
	    y2 = luaL_checknumber(L, 5);

	LUA_PUSH_WORLD();

	int scr_w, scr_h;
	SDL_GetWindowSize(win, &scr_w, &scr_h);

	SDL_SetRenderDrawColor(ren, 128, 0, 0, 255);

	int x, y;
	for(x=x1; x<=x2; x++) {
		for(y=y1; y<=y2; y++) {
			LUA_PUSH_METHOD("tile_walkable");
			lua_pushinteger(L, x);
			lua_pushinteger(L, y);
			if_call(3, 1);
			if(!lua_toboolean(L, -1)) {
				int x_ = x*8 + (scr_w/2/scale) + rx;
				int y_ = y*8 + (scr_h/2/scale) + ry;
				int w_ = 8;
				int h_ = 8;

				const SDL_Point pts[] = {
					{ x_-(w_/2), y_-(h_/2) },
					{ x_-(w_/2), y_+(h_/2) },
					{ x_+(w_/2), y_+(h_/2) },
					{ x_+(w_/2), y_-(h_/2) },
					{ x_-(w_/2), y_-(h_/2) }
				};
				SDL_RenderDrawLines(ren, pts, 5);
			}
			lua_pop(L, 1);
		}
	}
}


int cb_render_physics(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TTABLE);

	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	SDL_RenderClear(ren);

	render_physics_objects(L);
	render_physics_static(L);

	SDL_RenderPresent(ren);
	return 0;
}


int cb_window_tiles(lua_State* L)
{
	int w, h;
	SDL_GetWindowSize(win, &w, &h);

	lua_pushinteger(L, rx-(w/2 / scale / 8));
	lua_pushinteger(L, ry-(h/2 / scale / 8));
	lua_pushinteger(L, rx+(w/2 / scale / 8));
	lua_pushinteger(L, ry+(h/2 / scale / 8));

	return 4;
}
