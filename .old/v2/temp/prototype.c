// gcc -o prototype prototype.c -Wall -g `pkg-config --cflags --libs sdl2` -I/usr/include/chipmunk -lchipmunk

#include <stdlib.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <chipmunk.h>

typedef struct Object {
	int x, y, w, h;
	cpFloat angle;
	cpBody* body;
	cpShape* shape;
} Object;
Object* obj[500] = {};

void add_object(int x, int y, int w, int h)
{
	int i = 0;
	while(obj[i])
		i++;
	obj[i] = malloc(sizeof(Object));
	obj[i]->x = x;
	obj[i]->y = y;
	obj[i]->w = w;
	obj[i]->h = h;
}

int main()
{
	// setup
	int i;
	for(i=0; i<500; i++)
		obj[i] = NULL;

	// create window
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
		abort();
	SDL_Window *win = SDL_CreateWindow("window", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	if(!win)
		abort();
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
	if(!ren)
		abort();

	// add one object
	add_object(100, 100, 10, 10);

	// initialize physics
	cpVect gravity = cpv(1, 100);
	cpSpace *space = cpSpaceNew();
	cpSpaceSetGravity(space, gravity);

	// initialize physics objects
	i = 0;
	cpFloat mass = 1;
	while(obj[i]) {
		cpFloat moment = cpMomentForBox(mass, obj[i]->w, obj[i]->h);
		obj[i]->body = cpSpaceAddBody(space, cpBodyNew(mass, moment));
		cpBodySetPos(obj[i]->body, cpv(obj[i]->x, obj[i]->y));

		obj[i]->shape = cpSpaceAddShape(space, cpBoxShapeNew(obj[i]->body, obj[i]->w, obj[i]->h));
		cpShapeSetFriction(obj[i]->shape, 0.7);
		i++;
	}

	// add obstacles
	cpShape* obst = cpSegmentShapeNew(space->staticBody, cpv(104, 200), cpv(120, 210), 0);
	cpShapeSetFriction(obst, 1);
	cpSpaceAddShape(space, obst);

	// main loop
	SDL_Event e;
	while(1) {
		// check events
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT)
				exit(0);
		}

		// advance physics
		cpSpaceStep(space, 1.0/60.0);
		i = 0;
		int ok = 0;
		while(obj[i]) {
			cpVect pos = cpBodyGetPos(obj[i]->body);
			obj[i]->x = pos.x;
			obj[i]->y = pos.y;
			obj[i]->angle = cpBodyGetAngle(obj[i]->body);

			if(obj[i]->y > 300 && !ok) {
				cpBodyApplyImpulse(obj[0]->body, cpv(2, 0), cpv(2, 2));
				ok = 1;
			}

			i++;
		}

		// clear screen
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
		SDL_RenderClear(ren);

		// draw stuff
		SDL_SetRenderDrawColor(ren, 0, 128, 0, 255);
		i = 0;
		while(obj[i]) {
			double sw = obj[i]->w/2;
			double sh = obj[i]->h/2;
			double dir_rad = -obj[i]->angle;
			int p1x = (-sw * sin(dir_rad)) + (-sh * cos(dir_rad)) + obj[i]->x;
			int p1y = (-sw * cos(dir_rad)) - (-sh * sin(dir_rad)) + obj[i]->y;
			int p2x = ( sw * sin(dir_rad)) + (-sh * cos(dir_rad)) + obj[i]->x;
			int p2y = ( sw * cos(dir_rad)) - (-sh * sin(dir_rad)) + obj[i]->y;
			int p3x = ( sw * sin(dir_rad)) + ( sh * cos(dir_rad)) + obj[i]->x;
			int p3y = ( sw * cos(dir_rad)) - ( sh * sin(dir_rad)) + obj[i]->y;
			int p4x = (-sw * sin(dir_rad)) + ( sh * cos(dir_rad)) + obj[i]->x;
			int p4y = (-sw * cos(dir_rad)) - ( sh * sin(dir_rad)) + obj[i]->y;
			const SDL_Point pts[] = {
				(SDL_Point){ p1x, p1y },
				(SDL_Point){ p2x, p2y },
				(SDL_Point){ p3x, p3y },
				(SDL_Point){ p4x, p4y },
				(SDL_Point){ p1x, p1y },
			};
			SDL_RenderDrawLines(ren, &pts, 5);

			const SDL_Point pt2[] = {
				(SDL_Point){ 104, 200 },
				(SDL_Point){ 120, 200 },
				(SDL_Point){ 120, 210 },
				(SDL_Point){ 104, 210 },
				(SDL_Point){ 104, 200 },
			};
			SDL_RenderDrawLines(ren, &pt2, 5);
			i++;
		}
		SDL_RenderPresent(ren);

		SDL_Delay(30);
	}

	return 0;
}
