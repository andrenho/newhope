#include "ui.h"

#include <stdlib.h>
#include <string.h>

#include <lauxlib.h>

#include "luah.h"
#include "block.h"
#include "physics.h"
#include "minimap.h"
#include "vehicle.h"

UI ui;

typedef struct ScreenLimits {
	SDL_Renderer* ren;
	int x1, y1, x2, y2;
} ScreenLimits;

static SDL_Point circle_pixel[10000];
const double Z = 10;
int rx = 50, ry = 50;

static void visible_tiles(SDL_Window* win, int* x1, int *y1, int* x2, int* y2);
static void draw_visible_tiles(lua_State* L, ScreenLimits* s);
static void draw_static_shape(cpBody *body, cpShape *shape, void* data);
static void draw_vehicle(SDL_Renderer *ren, Vehicle* vehicle);
static void draw_circle(cpBody *body, cpShape *shape, void* data);



int ui_c_init(lua_State* L)
{
	// initialize SDL
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "\nUnable to initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	// initialize window
	if(!(ui.win = SDL_CreateWindow("NewHope", SDL_WINDOWPOS_CENTERED, 
			SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE))) {
		fprintf(stderr, "\nUnable to intialize window: %s\n", SDL_GetError());
		exit(1);
	}

	// initialize renderer
	if(!(ui.ren = SDL_CreateRenderer(ui.win, -1, SDL_RENDERER_SOFTWARE))) {
		fprintf(stderr, "\nUnable to intialize renderer: %s\n", SDL_GetError());
		exit(1);
	}

	// initialize TTF_Init
	if(TTF_Init() == -1) {
		fprintf(stderr, "\nError initializing SDL2_ttf: %s\n", TTF_GetError());
		exit(1);
	}

	// load font
	ui.main_font = TTF_OpenFont(DATADIR "/PressStart2P.ttf", 16);
	if(!ui.main_font) {
		fprintf(stderr, "\nUnable to load font: %s\n", TTF_GetError());
		exit(1);
	}

	// create minimap
	ui.mm = minimap_new(L, ui.ren, 250, 250);

	lua_pushlightuserdata(L, &ui);
	return 1;
}


int ui_now(lua_State* L)
{
	lua_pushinteger(L, SDL_GetTicks());
	return 1;
}


int ui_wait(lua_State* L)
{
	SDL_Delay(luaL_checkinteger(L, 2));
	return 0;
}


int ui_render(lua_State* L)
{
	SDL_SetRenderDrawColor(ui.ren, 255, 255, 255, 255);
	SDL_RenderClear(ui.ren);

	SDL_SetRenderDrawColor(ui.ren, 0, 0, 0, 255);

	// find visible tiles
	ScreenLimits s;
	s.ren = ui.ren;
	visible_tiles(ui.win, &s.x1, &s.y1, &s.x2, &s.y2);

	// draw visible tiles
	draw_visible_tiles(L, &s);

	// draw static objects
	cpBodyEachShape(space->staticBody, draw_static_shape, &s);

	// draw other objects
	int n = lua_objlen(L, 2);
	for(int i=1; i<=n; i++)
	{
		lua_rawgeti(L, 2, i);

		bool is_vehicle;
		LUA_FIELD(L, is_vehicle, "is_vehicle", boolean);

		if(is_vehicle) {
			Vehicle* vehicle;
			LUA_FIELD(L, vehicle, "c_ptr", userdata);
			draw_vehicle(ui.ren, vehicle);
		} else {
			cpBody* body;
			LUA_FIELD(L, body, "body", userdata);
			cpBodyEachShape(body, draw_circle, ui.ren);
		}
	}

	SDL_RenderPresent(ui.ren);
	return 0;
}


int ui_user_events(lua_State* L)
{
	lua_newtable(L);

	SDL_Event e;
	int i = 1;
	while(SDL_PollEvent(&e)) {
		switch(e.type) {
		case SDL_QUIT:
			lua_newtable(L);
			LUA_SET_FIELD(L, "quit", "event", string);
			lua_rawseti(L, -2, i++);
			break;
		case SDL_KEYDOWN:
			lua_newtable(L);
			LUA_SET_FIELD(L, "key", "event", string);
			LUA_SET_FIELD(L, SDL_GetKeyName(e.key.keysym.sym), "key", string);
			lua_rawseti(L, -2, i++);
			break;
		}
	}

	return 1;
}


int ui_keyboard_state(lua_State* L)
{
	const Uint8* k = SDL_GetKeyboardState(NULL);

	lua_createtable(L, 0, 5);
	LUA_SET_FIELD(L, k[SDL_SCANCODE_UP], "up", boolean);
	LUA_SET_FIELD(L, k[SDL_SCANCODE_DOWN], "down", boolean);
	LUA_SET_FIELD(L, k[SDL_SCANCODE_LEFT], "left", boolean);
	LUA_SET_FIELD(L, k[SDL_SCANCODE_RIGHT], "right", boolean);
	return 1;
}


int ui_center_screen(lua_State* L)
{
	int win_w, win_h;
	SDL_GetWindowSize(ui.win, &win_w, &win_h);

	// center screen on player
	double x = luaL_checknumber(L, 2);
	double y = luaL_checknumber(L, 3);
	rx = -x*Z + (win_w/2);
	ry = -y*Z + (win_h/2);

	return 0;
}


int ui_visible_tiles(lua_State* L)
{
	int win_w, win_h;
	SDL_GetWindowSize(ui.win, &win_w, &win_h);

	int x1, y1, x2, y2;
	visible_tiles(ui.win, &x1, &y1, &x2, &y2);

	lua_pushinteger(L, x1);
	lua_pushinteger(L, y1);
	lua_pushinteger(L, x2);
	lua_pushinteger(L, y2);

	return 4;
}


int ui_show_minimap(lua_State* L)
{
	int win_w, win_h;
	SDL_GetWindowSize(ui.win, &win_w, &win_h);

	SDL_SetRenderDrawColor(ui.ren, 0x96, 0x4b, 0x00, SDL_ALPHA_OPAQUE);
	SDL_Rect r = { win_w/2 - ui.mm->w/2 - 5, win_h/2 - ui.mm->h/2 - 5, 
		ui.mm->w + 10, ui.mm->h + 10 };
	SDL_RenderFillRect(ui.ren, &r);

	SDL_Rect r2 = { r.x+5, r.y+5, r.w-10, r.h-10 };
	SDL_RenderCopy(ui.ren, ui.mm->tx, NULL, &r2);

	minimap_draw_player(ui.mm, L, ui.ren, win_w, win_h);

	SDL_RenderPresent(ui.ren);

	SDL_Event e;
	for(;;) {
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_KEYDOWN) {
				return 0;
			}
		}
	}
}


int ui_clean_up(lua_State* L)
{
	SDL_DestroyRenderer(ui.ren);
	SDL_DestroyWindow(ui.win);
	SDL_Quit();
	return 0;
}


int ui_wrap_text(char* text, int chars)
{
	int i, len = strlen(text);
	int nl = 0, lines = 1;
	for(i=0; i<len; i++) {
		if(nl > chars) {
			lines++;
			nl = 0;
			while(text[i--] != ' ') { }
			text[i+1] = '\n';
		}
		nl++;
	}

	return lines;
}


/*
 * STATIC
 */
static void visible_tiles(SDL_Window* win, int* x1, int *y1, int* x2, int* y2)
{
	int win_w, win_h;
	SDL_GetWindowSize(win, &win_w, &win_h);

	*x1 = (-rx) / Z - 5;
	*y1 = (-ry) / Z - 5;
	*x2 = (-rx + (win_w)) / Z + 5;
	*y2 = (-ry + (win_h)) / Z + 5;
}


static void draw_visible_tiles(lua_State* L, ScreenLimits* s)
{
	SDL_Texture* tx = SDL_CreateTexture(s->ren, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_STATIC, 1, 1);

	// IMPORTANT: the last parameter should contain the tile list
	luaL_checktype(L, -1, LUA_TTABLE);

	int x, y;
	for(x=s->x1; x<=s->x2; x++) for(y=s->y1; y<=s->y2; y++) {
		// find tile ID
		lua_rawgeti(L, -1, x);  		// t[x]
		lua_rawgeti(L, -1, y);  		// t[x][y]
		lua_rawgeti(L, -1, 1);			// t[x][y][1]
		int block = lua_tointeger(L, -1);

		// draw tile
		switch(block) {
		case GRASS:
			SDL_SetRenderDrawColor(s->ren, 0, 128, 0, 0); break;
		case WATER:
			SDL_SetRenderDrawColor(s->ren, 0, 0, 255, 0); break;
		case FLOOR:
			SDL_SetRenderDrawColor(s->ren, 255, 0, 0, 0); break;
		default:
			SDL_SetRenderDrawColor(s->ren, 255, 255, 255, 0); break;
		}
		SDL_RenderDrawPoint(s->ren, (x+0.5)*Z+rx, (y+0.5)*Z+ry);
		SDL_RenderDrawPoint(s->ren, (x+0.5)*Z+rx, (y+0.5)*Z+ry+1);
		SDL_RenderDrawPoint(s->ren, (x+0.5)*Z+rx+1, (y+0.5)*Z+ry+1);

		lua_pop(L, 3);
	}

	SDL_DestroyTexture(tx);
}


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
