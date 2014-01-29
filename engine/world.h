// Copyright 2014 <Imperial Software>

#ifndef ENGINE_WORLD_H_
#define ENGINE_WORLD_H_

class World {
public:
	World(int x1, int y1, int x2, int y2);
	~World();

	void Initialize();

	inline class Hero& Hero() const { return *hero; }

private:
	World(const World&);
	World& operator=(const World&);

	int x1, y1, x2, y2;
	class Hero* hero;
};

#endif  // ENGINE_WORLD_H_
