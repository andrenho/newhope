// gcc -g -Wall -o testbed test.c `sdl2-config --cflags --libs` -I/usr/include/chipmunk -lchipmunk

#include <SDL2/SDL.h>
#include <chipmunk.h>

#include <stdlib.h>
#include <stdio.h>

SDL_Renderer* ren;

cpSpace* space;
cpBody* tire[4];

void init_physics()
{
	// setup space
	space = cpSpaceNew();

	// setup tires
	int i;
	for(i=0; i<1; i++) {
		cpFloat x = 0, y = 0, w = 0.5, h = 1.75;
		cpFloat mass = 50;
		cpFloat moment = cpMomentForBox(mass, w, h);
		tire[i] = cpSpaceAddBody(space, cpBodyNew(mass, moment));
		cpSpaceAddShape(space, cpBoxShapeNew2(tire[i], 
					cpBBNew(x-(w/2), y-(h/2), x+(w/2), y+(h/2))));
		cpBodySetAngle(tire[0], 2);
	}
}

void draw_shape(cpBody* body, cpShape* shape, void* data)
{
	int n = cpPolyShapeGetNumVerts(shape); 
	SDL_Point* pts = calloc(sizeof(SDL_Point), n+1);

	int i;
	for(i=0; i<n; i++) {
		cpVect p = cpPolyShapeGetVert(shape, i);
		pts[i] = (SDL_Point) { p.x*10+50, p.y*10+50 };
		if(i == 0)
			pts[n] = pts[i];
	}
	SDL_RenderDrawLines(ren, pts, n+1);

	free(pts);
}

void render_objects()
{
	int i;
	for(i=0; i<1; i++) {
		cpBodyEachShape(tire[i], draw_shape, NULL);
	}
}

int main()
{
	SDL_Window* win;

	// initialize SDL
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "\nUnable to initialize SDL: %s\n", SDL_GetError());
	}

	// initialize window and renderer
	if(SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_RESIZABLE, &win, 
				&ren) != 0) {
		fprintf(stderr, "\nUnable to intialize window: %s\n", SDL_GetError());
	}

	init_physics();

	// main lopp
	for(;;) {
		Uint32 t = SDL_GetTicks() + 1000/60;

		// physics step
		cpSpaceStep(space, 1.0/60.0);

		// events
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			switch(e.type) {
			case SDL_QUIT:
				exit(0);
				break;
			}
		}

		// render
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);

		SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
		render_objects();

		SDL_RenderPresent(ren);

		// time
		if(SDL_GetTicks() < t) {
			SDL_Delay(t - SDL_GetTicks());
		}
	}

	return 0;
}
