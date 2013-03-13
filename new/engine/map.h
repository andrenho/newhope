#ifndef GAME_MAP_H
#define GAME_MAP_H

typedef enum Terrain { t_GRASS, t_WATER } Terrain;

class Map {
public:
	virtual ~Map() { }

	virtual enum Terrain Terrain(int x, int y) const = 0;

	int W() const { return w_; }
	int H() const { return h_; }

protected:
	int w_, h_;
};

#endif
