#ifndef GAME_GAMEMODE_H
#define GAME_GAMEMODE_H

class GameMode {
public:
	virtual ~GameMode() { 
		delete map_;
	}

	inline class Map& Map() { return *map_; }

protected:
	class Map* map_;
};

#endif

