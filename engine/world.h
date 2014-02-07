#ifndef ENGINE_WORLD_H_
#define ENGINE_WORLD_H_

#include <map>
#include <vector>
#include "engine/block.h"
#include "engine/city.h"
#include "engine/mapgen.h"

class Object;

class World final {
public:
	World(int x1, int y1, int x2, int y2, unsigned int seedp);
	~World();

	void Initialize();
	void Step();

	int Tiles(const Block* (&block)[10], int x, int y) const;
	bool IsTileWalkable(int x, int y) const;
	void Limits(int& x1, int& y1, int& x2, int& y2) const;

	inline class Hero& Hero() const { return *hero; }
	inline std::vector<Object*> const& Objects() const { return objects; }
	inline std::vector<City*> const& Cities() const { return cities; }
	inline std::vector<std::vector<Point>> const& Rivers() const { return mapgen->Rivers(); }

	inline struct cpSpace* SpacePhysics() const { return space; }

	double Random() const;

private:
	World(const World&);
	World& operator=(const World&);

	void CreateCities();
	void AddObject(Object* obj);
	void AddWorkers();
	void AddStaticObjects();
	void AddStaticObject(double x, double y, double w, double h);

	static void FreeStaticShape(struct cpBody *body, struct cpShape *shape, 
			void* data);
	static int CollisionCallback(struct cpArbiter *arb, 
			struct cpSpace *space, void *data);

	int x1, y1, x2, y2;
	mutable unsigned int seedp;
	class Hero* hero;
	struct cpSpace* space;

	class MapGen* mapgen;

	std::vector<Object*> objects;
	std::vector<class City*> cities;

	std::map<struct cpBody*, Object*> physics_ptr;
};

#endif  // ENGINE_WORLD_H_
