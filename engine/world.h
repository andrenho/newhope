// Copyright 2014 <Imperial Software>

#ifndef ENGINE_WORLD_H_
#define ENGINE_WORLD_H_

#include <map>
#include <vector>
#include "engine/block.h"

class Object;

class World {
public:
	World(int x1, int y1, int x2, int y2);
	~World();

	void Initialize();
	void Step();

	int Tiles(const Block* (&block)[10], int x, int y);
	bool IsTileWalkable(int x, int y);

	inline class Hero& Hero() const { return *hero; }
	inline std::vector<Object*> const& Objects() { return objects; }
	inline struct cpSpace* SpacePhysics() const { return space; }

private:
	void AddObject(Object* obj);
	void AddStaticObjects();
	void AddStaticObject(double x, double y, double w, double h);

	static void FreeStaticShape(struct cpBody *body, struct cpShape *shape, 
			void* data);
	static int CollisionCallback(struct cpArbiter *arb, 
			struct cpSpace *space, void *data);

	World(const World&);
	World& operator=(const World&);

	int x1, y1, x2, y2;
	class Hero* hero;
	struct cpSpace* space;
	std::vector<Object*> objects;
	std::vector<class City*> cities;

	std::map<struct cpBody*, Object*> physics_ptr;
	class MapGen* mapgen;
};

#endif  // ENGINE_WORLD_H_
