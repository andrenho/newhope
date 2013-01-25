#ifndef UI_FIRSTPLANE_H
#define UI_FIRSTPLANE_H

#include "util/defines.h"
#include "util/point.h"
#include "util/rect.h"
#include "world/world.h"

class GraphicLibrary;
class Resources;
class Person;
class UI;
class World;


class FirstPlane {
public:
	FirstPlane(const World& world, const GraphicLibrary& video, 
			const Resources& res, const UI& ui)
		: world(world), video(video), res(res), ui(ui) { }
	~FirstPlane();

	void DrawObjectsInFrontOf(const Person& person) const;

private:
	void DrawFrontTile(Point<int> p, double feet) const;

	const World& world;
	const GraphicLibrary& video;
	const Resources& res;
	const UI& ui;

	DISALLOW_COPY_AND_ASSIGN(FirstPlane);
};

#endif
