#include "ui/cityengine.h"

#include "libs/image.h"
#include "libs/graphiclibrary.h"
#include "ui/firstplane.h"
#include "ui/resource.h"
#include "ui/ui.h"
#include "world/world.h"
#include "world/person.h"

CityEngine::CityEngine(const World& world, const GraphicLibrary& video, 
	 	       const Resources& res)
	: world(world), video(video), res(res)
{
}


CityEngine::~CityEngine()
{
}
