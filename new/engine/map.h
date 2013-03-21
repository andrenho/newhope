#ifndef GAME_MAP_H
#define GAME_MAP_H

typedef enum Terrain { t_GRASS, t_WATER, t_DIRT, t_DIRT2, t_SNOW, t_LAVA,
		       t_DESERT, t_TUNDRA, t_LAVAROCK } Terrain;

class Map {
public:
	Map() : w_(0), h_(0) { }
	virtual ~Map() { }

	virtual enum Terrain Terrain(int x, int y) const = 0;

	int W() const { return w_; }
	int H() const { return h_; }

protected:
	int w_, h_;
};

#endif
