#include"wireframe.h"

#include <stdbool.h>
#include <lauxlib.h>

#include "luah.h"
#include "physics.h"
#include "vehicle.h"

// TODO
// TODO - restrict all to visible area
// TODO

const double Z = 8;
int rx = 50, ry = 50;

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
	SDL_Renderer* ren = (SDL_Renderer*)data;
	SDL_SetRenderDrawColor(ren, 150, 0, 0, 255);
	cpBB bb = cpShapeGetBB(shape);
	SDL_Rect rect = { bb.l*Z+rx, bb.b*Z+ry, (bb.r-bb.l)*Z, (bb.t-bb.b)*Z };
	SDL_RenderDrawRect(ren, &rect);
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

void wireframe_render(lua_State* L, SDL_Renderer* ren)
{
	// draw static bodies
	cpBodyEachShape(space->staticBody, draw_static_shape, ren);

	// draw other objects
	int n = luaL_len(L, 2);
	for(int i=1; i<=n; i++)
	{
		lua_rawgeti(L, 2, i);

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
