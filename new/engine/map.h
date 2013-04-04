#ifndef GAME_MAP_H
#define GAME_MAP_H

typedef enum Terrain { t_GRASS, t_WATER, t_DIRT, t_DIRT2, t_SNOW, t_LAVA,
		       t_DESERT, t_TUNDRA, t_LAVAROCK } Terrain;

typedef enum TreeType { tt_NONE, tt_PINE, tt_CONIFER, tt_BROADLEAF } TreeType;

class Map {
public:
	Map();
	virtual ~Map() { }

	virtual enum Terrain Terrain(Point<int> p) const = 0;
	virtual int Special(Point<int> p) const;
	virtual TreeType Tree(Point<int> p) const = 0;

	int W() const { return w_; }
	int H() const { return h_; }

protected:
	int w_, h_;
	int randspecial_[100][100];
};

#endif
