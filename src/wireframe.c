#include"wireframe.h"

#include <stdbool.h>
#include <string.h>

#include <lauxlib.h>

#include "luah.h"
#include "physics.h"
#include "vehicle.h"
#include "ui.h"

// TODO
// TODO - restrict all to visible area
// TODO

static void visible_tiles(SDL_Window* win, int* x1, int *y1, int* x2, int* y2);
typedef struct ScreenLimits {
	SDL_Renderer* ren;
	int x1, y1, x2, y2;
} ScreenLimits;

const double Z = 8;
int rx = 50, ry = 50;

extern char *strdup(const char *s);

SDL_Point circle_pixel[10000];
static void render_circle(SDL_Renderer* ren, double x1, double y1, double r)
{
	int x=r, y=0;
	int radiusError = 1-x;
	int i = 0;

	while(x >= y) {
		circle_pixel[i++] = (const SDL_Point){  x + x1,  y + y1 };
		circle_pixel[i++] = (const SDL_Point){  y + x1,  x + y1 };
		circle_pixel[i++] = (const SDL_Point){ -x + x1,  y + y1 };
		circle_pixel[i++] = (const SDL_Point){ -y + x1,  x + y1 };
		circle_pixel[i++] = (const SDL_Point){ -x + x1, -y + y1 };
		circle_pixel[i++] = (const SDL_Point){ -y + x1, -x + y1 };
		circle_pixel[i++] = (const SDL_Point){  x + x1, -y + y1 };
		circle_pixel[i++] = (const SDL_Point){  y + x1, -x + y1 };
		y++;
		if(radiusError<0)
			radiusError += 2*y+1;
		else {
			x--;
			radiusError += 2*(y-x+1);
		}
	}

	SDL_RenderDrawPoints(ren, circle_pixel, i);
}


static void draw_circle(cpBody *body, cpShape *shape, void* data)
{
	SDL_Renderer* ren = (SDL_Renderer*)data;
	cpVect pos = cpBodyGetPos(body);
	cpFloat r = cpCircleShapeGetRadius(shape);
	SDL_SetRenderDrawColor(ren, 0, 128, 0, 255);
	render_circle(ren, pos.x*Z+rx, pos.y*Z+ry, r*Z);
}


static void draw_static_shape(cpBody *body, cpShape *shape, void* data)
{
	ScreenLimits* s = (ScreenLimits*)data;
	cpBB bb = cpShapeGetBB(shape);
	if(bb.l < s->x1 || bb.l > s->x2 || bb.t < s->y1 || bb.t > s->y2) {
		// out of bounds
		return;
	}
	SDL_SetRenderDrawColor(s->ren, 150, 0, 0, 255);
	SDL_Rect rect = { bb.l*Z+rx, bb.b*Z+ry, (bb.r-bb.l)*Z, (bb.t-bb.b)*Z };
	SDL_RenderDrawRect(s->ren, &rect);
}


static void draw_shape(SDL_Renderer* ren, cpBody* body, cpShape* shape)
{
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);

	// get body info
	cpVect v = cpBodyGetPos(body);
	cpFloat angle = cpBodyGetAngle(body);
	cpVect rot = cpvforangle(angle);

	// get vectors
	int n = cpPolyShapeGetNumVerts(shape); 
	SDL_Point* pts = calloc(sizeof(SDL_Point), n+1);

	// rotate vectors
	int i;
	for(i=0; i<n; i++) {
		cpVect p = cpPolyShapeGetVert(shape, i);
		cpVect vr = cpvrotate(cpv(p.x,p.y), rot);
		pts[i] = (SDL_Point) { (vr.x+v.x)*Z+rx, (vr.y+v.y)*Z+ry };
		if(i == 0)
			pts[n] = pts[i];
	}

	// draw
	SDL_RenderDrawLines(ren, pts, n+1);

	free(pts);
}


static void draw_vehicle(SDL_Renderer *ren, Vehicle* vehicle)
{
	draw_shape(ren, vehicle->vehicle_body, vehicle->vehicle_shape);
	draw_shape(ren, vehicle->rear_wheel_body, vehicle->rear_wheel_shape);
	draw_shape(ren, vehicle->front_wheel_body, vehicle->front_wheel_shape);
}


void wireframe_render(lua_State* L, SDL_Window* win, SDL_Renderer* ren)
{
	int win_w, win_h;
	SDL_GetWindowSize(win, &win_w, &win_h);

	// center screen on player
	double x = luaL_checknumber(L, 2);
	double y = luaL_checknumber(L, 3);
	rx = -x*Z + (win_w/2);
	ry = -y*Z + (win_h/2);

	// draw static bodies
	ScreenLimits s;
	s.ren = ren;
	visible_tiles(win, &s.x1, &s.y1, &s.x2, &s.y2);
	cpBodyEachShape(space->staticBody, draw_static_shape, &s);

	// draw other objects
	int n = lua_objlen(L, 4);
	for(int i=1; i<=n; i++)
	{
		lua_rawgeti(L, 4, i);

		bool is_vehicle;
		LUA_FIELD(L, is_vehicle, "is_vehicle", boolean);

		if(is_vehicle) {
			Vehicle* vehicle;
			LUA_FIELD(L, vehicle, "c_ptr", userdata);
			draw_vehicle(ren, vehicle);
		} else {
			cpBody* body;
			LUA_FIELD(L, body, "body", userdata);
			cpBodyEachShape(body, draw_circle, ren);
		}
	}
}


int wireframe_message(lua_State* L, SDL_Window* win, SDL_Renderer* ren, 
		TTF_Font* font)
{
	// get screen size
	int win_w, win_h;
	SDL_GetWindowSize(win, &win_w, &win_h);

	// check parameters
	luaL_checktype(L, 1, LUA_TTABLE);
	char* text = strdup(luaL_checkstring(L, 2));

	// display message
	int advance;
	TTF_GlyphMetrics(font, 'A', NULL, NULL, NULL, NULL, &advance);
	int lines = ui_wrap_text(text, (win_w-50) / advance);
	SDL_Rect r = { 0, win_h - (lines * TTF_FontLineSkip(font)) - 50, 
		win_w, (lines * TTF_FontLineSkip(font)) + 50 };
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	SDL_RenderFillRect(ren, &r);
	int y = win_h - (lines * TTF_FontLineSkip(font)) - 25;
	char *token;
#ifdef __MINGW32__
	while((token = strtok(text, "\n")) != NULL) {
#else
	char* saveptr = NULL;
	while((token = strtok_r(text, "\n", &saveptr)) != NULL) {
#endif
		SDL_Surface* sf = TTF_RenderUTF8_Solid(font, token, 
				(SDL_Color){255,255,255});
		SDL_Texture* txt = SDL_CreateTextureFromSurface(ren, sf);
		SDL_RenderCopy(ren, txt, NULL, &(SDL_Rect){ 25, y, sf->w, sf->h });
		SDL_FreeSurface(sf);
		SDL_DestroyTexture(txt);
		y += TTF_FontLineSkip(font);
		text = NULL;
	}
	SDL_RenderPresent(ren);
	
	// wait for keypress
	for(;;) {
		SDL_Event e;
		SDL_PollEvent(&e);
		const Uint8* k = SDL_GetKeyboardState(NULL);
		if(k[SDL_SCANCODE_SPACE])
			break;
		SDL_Delay(1000.0/60.0);
	}
	for(;;) {
		SDL_Event e;
		SDL_PollEvent(&e);
		const Uint8* k = SDL_GetKeyboardState(NULL);
		if(!k[SDL_SCANCODE_SPACE])
			break;
		SDL_Delay(1000.0/60.0);
	}

	free(text);

	return 0;
}


static void visible_tiles(SDL_Window* win, int* x1, int *y1, int* x2, int* y2)
{
	int win_w, win_h;
	SDL_GetWindowSize(win, &win_w, &win_h);

	*x1 = (-rx) / Z - 5;
	*y1 = (-ry) / Z - 5;
	*x2 = (-rx + (win_w)) / Z + 5;
	*y2 = (-ry + (win_h)) / Z + 5;
}


int wireframe_visible_tiles(lua_State* L, SDL_Window* win)
{
	int win_w, win_h;
	SDL_GetWindowSize(win, &win_w, &win_h);

	int x1, y1, x2, y2;
	visible_tiles(win, &x1, &y1, &x2, &y2);

	lua_pushinteger(L, x1);
	lua_pushinteger(L, y1);
	lua_pushinteger(L, x2);
	lua_pushinteger(L, y2);

	return 4;
}
