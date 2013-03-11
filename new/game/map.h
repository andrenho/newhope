#ifndef GAME_MAP_H
#define GAME_MAP_H

typedef enum Terrain { t_GRASS, t_WATER } Terrain;

class Map {
public:
	enum Terrain Terrain(int x, int y) const { 
		if(x == 2 && y == 2) {
			return t_WATER;
		} else {
			return t_GRASS; 
		}
	}
	int W() const { return 5000; }
	int H() const { return 5000; }
};

#endif
