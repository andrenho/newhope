#ifndef GAME_MAP_H
#define GAME_MAP_H

typedef enum Terrain { t_GRASS } Terrain;

class Map {
public:
	enum Terrain Terrain(int x, int y) const { return t_GRASS; }
	int W() const { return 5000; }
	int H() const { return 5000; }
};

#endif
