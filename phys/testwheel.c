// gcc -g -Wall -o testbed test.c `sdl2-config --cflags --libs` -I/usr/include/chipmunk -lchipmunk

#include <SDL2/SDL.h>
#include <chipmunk.h>

#include <stdlib.h>
#include <stdio.h>

SDL_Renderer* ren;

typedef struct Controls {
	int forward;
	int back;
	int right;
	int left;
} Controls;

cpSpace* space;
cpBody* tire[4];

Controls controls = { 0, 0, 0, 0 };

/*************
 *           *
 *  PHYSICS  *
 *           *
 *************/

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
		cpSpaceReindexShapesForBody(space, tire[0]);
		cpBodySetAngle(tire[i], -M_PI/6);
		//cpBodySetVel(tire[i], cpv(0, 10));
		//cpBodyApplyImpulse(tire[i], cpv(0, 1000), cpvzero);
	}
}

cpVect lateral_velocity(int n)
{
	cpVect normal = cpvforangle(cpBodyGetAngle(tire[n]));
	return cpvmult(normal, cpvdot(normal, cpBodyGetVel(tire[n])));
}

cpVect forward_velocity(int n)
{
	cpVect normal = cpvperp(cpvforangle(cpBodyGetAngle(tire[n])));
	return cpvmult(normal, cpvdot(normal, cpBodyGetVel(tire[n])));
}

void update_friction(int n)
{
	// kill lateral velocity
	const cpFloat max_lateral_impulse = 300;
	cpVect impulse = cpvmult(cpvneg(lateral_velocity(n)), cpBodyGetMass(tire[n]));
	//printf("%f\n", cpvlength(impulse));
	if(cpvlength(impulse) > max_lateral_impulse)
		impulse = cpvmult(impulse, max_lateral_impulse / cpvlength(impulse));
	cpBodyApplyImpulse(tire[n], impulse, cpvzero);


	// TODO - kill angular velocity?
	cpFloat inertia = cpBodyGetMoment(tire[n]);
	cpFloat av = cpBodyGetAngVel(tire[n]);
	if(av != 0)
		cpBodySetAngVel(tire[n], av / 1.2);
	
	// apply drag
	cpVect forward_normal = forward_velocity(n);
	cpFloat forward_speed = cpvlength(forward_normal);
	if(forward_speed < 1) {
		cpBodySetVel(tire[n], cpvzero);
	} else {
		forward_normal = cpvnormalize(forward_normal);
		cpFloat drag = -1 * forward_speed;
		cpBodyApplyImpulse(tire[n], cpvmult(forward_normal, drag), cpvzero);
	}
}

void update_drive()
{
	const cpFloat max_forward_speed = 150;
	const cpFloat max_backward_speed = -20;
	const cpFloat max_drive_force = 100;

	int i;
	for(i=0; i<1; i++) {
		cpFloat desired_speed = 0;

		// find desired speed
		if(controls.forward)
			desired_speed = max_forward_speed;
		else if(controls.back)
			desired_speed = max_backward_speed;

		// find speed
		cpVect forward_normal = cpvperp(cpvforangle(cpBodyGetAngle(tire[i])));
		cpFloat speed = cpvdot(forward_velocity(i), forward_normal);

		// apply force
		cpFloat force = 0;
		if(desired_speed > speed)
			force = max_drive_force;
		else if(desired_speed < speed)
			force = -max_drive_force;
		else
			return;
		cpBodyApplyImpulse(tire[i], cpvmult(forward_normal, force), cpvzero);
	}
}

void update_turn()
{
	int i;
	for(i=0; i<1; i++) {
		cpFloat desired_torque = 0;
		if(controls.left)
			desired_torque = -1100;
		else if(controls.right)
			desired_torque = 1100;
		cpBodySetTorque(tire[i], desired_torque);
	}
}

void debug_physics()
{
}

/************
 *          *
 *    UI    *
 *          *
 ************/

void draw_shape(cpBody* body, cpShape* shape, void* data)
{
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
		pts[i] = (SDL_Point) { (vr.x+v.x)*10+50, (vr.y+v.y)*10+50 };
		if(i == 0)
			pts[n] = pts[i];
	}

	// draw
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
		update_friction(0);
		update_drive();
		update_turn();
		cpSpaceStep(space, 1.0/60.0);
		debug_physics();

		// events
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			switch(e.type) {
			case SDL_QUIT:
				exit(0);
				break;
			}
		}
		const Uint8* k = SDL_GetKeyboardState(NULL);
		controls.forward = k[SDL_SCANCODE_UP];
		controls.back = k[SDL_SCANCODE_DOWN];
		controls.left = k[SDL_SCANCODE_LEFT];
		controls.right = k[SDL_SCANCODE_RIGHT];

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
