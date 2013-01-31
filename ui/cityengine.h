#ifndef UI_CITYENGINE_H
#define UI_CITYENGINE_H

#include <queue>
#include "util/defines.h"
#include "util/point.h"
#include "util/rect.h"
#include "world/world.h"

class Building;
class City;
class GraphicLibrary;
class Image;
class Resources;
class Person;
struct Tiles;


class CityEngine {
public:
	CityEngine(const World& world, const GraphicLibrary& video, 
			const Resources& res);
	~CityEngine();

	void AddBuildings(Point<int> p, std::queue<const Image*>& st, 
			double feet) const;
private:
	void AddBuildingTile(Point<int> p, std::queue<const Image*>& st, 
			const City& city, double feet) const;
	void AddBackWall(std::queue<const Image*>& st, const Tiles& tile) const;
	void AddWall(std::queue<const Image*>& st, const Tiles& tile) const;
	void AddDoorFrames(std::queue<const Image*>& st, 
			const Tiles& tile) const;

	const World& world;
	const GraphicLibrary& video;
	const Resources& res;

	DISALLOW_COPY_AND_ASSIGN(CityEngine);
};

#endif


