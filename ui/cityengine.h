#ifndef UI_CITYENGINE_H
#define UI_CITYENGINE_H

#include "util/defines.h"
#include "util/rect.h"
#include "world/world.h"

class GraphicLibrary;
class Resources;
class Person;


class CityEngine {
public:
	CityEngine(const World& world, const GraphicLibrary& video, 
			const Resources& res);
	~CityEngine();

private:
	const World& world;
	const GraphicLibrary& video;
	const Resources& res;

	DISALLOW_COPY_AND_ASSIGN(CityEngine);
};

#endif


