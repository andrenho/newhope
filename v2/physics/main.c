#include <stdio.h>
#include <chipmunk.h>

int main()
{
	cpVect gravity = cpv(0, -100);

	cpSpace *space = cpSpaceNew();
	cpSpaceSetGravity(space, gravity);
	
	cpShape *ground = cpSegmentShapeNew(space->staticBody, cpv(-20,5), cpv(20,5), 0);
	cpShapeSetFriction(ground, 1);
	cpSpaceAddShape(space, ground);

	cpFloat radius = 5;
	cpFloat mass = 1;

	cpFloat moment = cpMomentForCircle(mass, 0, radius, cpvzero);
	
	cpBody *ballBody = cpSpaceAddBody(space, cpBodyNew(mass, moment));
	cpBodySetPos(ballBody, cpv(0, 15));

	cpShape *ballShape = cpSpaceAddShape(space, cpCircleShapeNew(ballBody, radius, cpvzero));
	cpShapeSetFriction(ballShape, 0.7);

	cpFloat timeStep = 1.0/60.0;

	cpFloat time;
	for(time=0; time<2; time+=timeStep) {
		cpVect pos = cpBodyGetPos(ballBody);
		cpVect vel = cpBodyGetVel(ballBody);
		printf(
		  "Time is %5.2f. ballBody is at (%5.2f, %5.2f). It's velocity is (%5.2f, %5.2f)\n",
		  time, pos.x, pos.y, vel.x, vel.y);
		cpSpaceStep(space, timeStep);
	}

	cpShapeFree(ballShape);
	cpBodyFree(ballBody);
	cpShapeFree(ground);
	cpSpaceFree(space);

	return 0;
}
