#ifndef UI_CHARENGINE_H
#define UI_CHARENGINE_H

#include "util/defines.h"
#include "world/world.h"

class GraphicLibrary;
class Resources;
class Person;
class UI;

typedef enum { TREE, PERSON } ObjType;
struct CharObject {
	CharObject(int y, ObjType o) : 
		y(y), objType(o) { }
	const int y;
	const ObjType objType;
};
struct CharObjPerson : public CharObject {
	CharObjPerson(int y, ObjType o, const Person* person) 
		: CharObject(y, o), person(person) { }
	const Person* person;
};
struct CharObjTree : public CharObject {
	CharObjTree(int y, ObjType o, Point<int> tile, TreeType type) 
		: CharObject(y, o), tile(tile), type(type) { }
	const Point<int> tile;
	const TreeType type;
};

class CharEngine {
public:
	CharEngine(const World& world, const GraphicLibrary& video, 
			const Resources& res, const UI& ui)
		: world(world), video(video), res(res), ui(ui) { }
	~CharEngine();

	void PrepareFrame(int scr_w, int scr_h) const;
	void Draw() const;

private:
	void DrawTree(const CharObjTree* obj) const;
	void DrawPerson(const Person* person) const;

	const World& world;
	const GraphicLibrary& video;
	const Resources& res;
	const UI& ui;

	mutable std::vector<const CharObject*> obj_frame;

	DISALLOW_COPY_AND_ASSIGN(CharEngine);
};

#endif

