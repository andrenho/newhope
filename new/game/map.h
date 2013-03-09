#ifndef GAME_MAP_H
#define GAME_MAP_H

typedef enum Terrain { t_GRASS } Terrain;

class Map {
	enum Terrain Terrain(int x, int y) { return t_GRASS; }
};

#endif
