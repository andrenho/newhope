#ifndef WORLD_H
#define WORLD_H

class World {
public:
	World(int x1, int y1, int x2, int y2);
	~World();

	void Initialize();

private:
	World(const World&);
	World& operator=(const World&);

	int x1, y1, x2, y2;
};

#endif
