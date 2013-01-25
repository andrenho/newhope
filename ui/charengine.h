#ifndef UI_CHARENGINE_H
#define UI_CHARENGINE_H

#include "util/defines.h"
#include "util/rect.h"
#include "world/world.h"

class FirstPlane;
class GraphicLibrary;
class Resources;
class Person;
class UI;


class CharEngine {
public:
	CharEngine(const World& world, const GraphicLibrary& video, 
			const Resources& res, const UI& ui);
	~CharEngine();

	void Draw(int scr_w, int scr_h) const;
	static const Rect TilesAffected(Person const& person);

private:
	void DrawPerson(const Person& person) const;

	const World& world;
	const GraphicLibrary& video;
	const Resources& res;
	const UI& ui;
	const FirstPlane* first_plane;

	mutable std::vector<const Person*> people_frame;

	DISALLOW_COPY_AND_ASSIGN(CharEngine);
};

#endif

