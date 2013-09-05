#ifndef ENGINE_OBJECT_H
#define ENGINE_OBJECT_H

typedef struct Object {
	enum { NONE, WALL, DOOR } type;
	int code;
} Object;

//
// codes
//

// WALL
enum { WALL_N, WALL_S, WALL_E, WALL_W, WALL_NW, WALL_NE, WALL_SW, WALL_SE };

#endif
