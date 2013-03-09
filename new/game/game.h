#ifndef GAME_GAME_H
#define GAME_GAME_H

class Game {
public:
	Game(int w, int h);
	inline class Map const& Map() { return *map_; }

private:
	class Map* map_;
};

#endif
