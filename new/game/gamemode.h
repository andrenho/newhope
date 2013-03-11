#ifndef GAME_GAMEMODE_H
#define GAME_GAMEMODE_H

class GameMode {
public:
	virtual ~GameMode() { }
	inline class Map const& Map() { return *map_; }

protected:
	class Map* map_;
};

#endif

