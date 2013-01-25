#include "ui/charengine.h"

#include <climits>
#include <cstdlib>
#include <string>
#include <sstream>
using namespace std;

#include "libs/image.h"
#include "libs/graphiclibrary.h"
#include "ui/firstplane.h"
#include "ui/resource.h"
#include "ui/ui.h"
#include "world/world.h"
#include "world/person.h"

CharEngine::CharEngine(const World& world, const GraphicLibrary& video, 
	 	       const Resources& res, const UI& ui)
	: world(world), video(video), res(res), ui(ui),
	  first_plane(new FirstPlane(world, video, res, ui))
{
}


CharEngine::~CharEngine()
{
	people_frame.clear();
	delete first_plane;
}


void 
CharEngine::Draw(int scr_w, int scr_h) const
{
	// clear frame
	people_frame.clear();

	// add people
	for(const auto& person: world.People) {
		Point<int> p(ui.TileToRel(person->Pos));
		if(p.x >= -TileSize && p.y >= -TileSize 
		&& p.x < scr_w + TileSize && p.y < scr_h + TileSize) {
			people_frame.push_back(person);
		}
	}

	// sort frame
	sort(people_frame.begin(), people_frame.end(),
	[](const Person* const& p1, const Person* const& p2) -> bool { 
		return p1->Pos.y > p2->Pos.y;
	});

	// draw people
	while(!people_frame.empty()) {
		DrawPerson(*people_frame[0], scr_w, scr_h);
		people_frame.erase(people_frame.begin());
	}

	if(!people_frame.empty())
		abort();
}


void 
CharEngine::DrawPerson(const Person& person, int scr_w, int scr_h) const
{
	// body
	string body("male");

	// clothes
	string clothes("pants");

	// direction
	string direction(1, person.Facing);

	// step
	std::stringstream s;
	s << person.Step() % 8;
	string step(s.str());

	// find position
	Point<int> scr(ui.TileToRel(person.Pos));

	// create image
	string charimage(body + "_" + direction + "_" + step);
	res[charimage]->Blit(*video.Window, scr);

	string charclothes(clothes + "_" + direction + "_" + step);
	res[charclothes]->Blit(*video.Window, scr);

	// draw other objects
	first_plane->DrawObjectsInFrontOf(person);
}


const Rect 
CharEngine::TilesAffected(Person const& person)
{
	int x = person.Pos.x,
	    y = person.Pos.y;
	return Rect(x-1, y-1, 1, 1);
}