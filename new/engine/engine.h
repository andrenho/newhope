#ifndef ENGINE_ENGINE_H
#define ENGINE_ENGINE_H

class Engine {
public:
	virtual ~Engine() { 
		delete map_;
	}

	inline class Map& Map() { return *map_; }

protected:
	class Map* map_;
};

#endif

