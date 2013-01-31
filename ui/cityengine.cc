#include "ui/cityengine.h"

#include <string>
using namespace std;

#include "libs/image.h"
#include "libs/graphiclibrary.h"
#include "ui/firstplane.h"
#include "ui/resource.h"
#include "ui/ui.h"
#include "world/building.h"
#include "world/city.h"
#include "world/mapbuild.h"
#include "world/world.h"
#include "world/person.h"

struct Tiles {
	std::string nw, n, ne, w, c, e, sw, s, se;
};


CityEngine::CityEngine(const World& world, const GraphicLibrary& video, 
	 	       const Resources& res)
	: world(world), video(video), res(res)
{
}


CityEngine::~CityEngine()
{
}


void 
CityEngine::AddBuildings(Point<int> p, std::queue<const Image*>& st, 
		double feet) const
{
	for(const auto& city: world.map->cities) {
		if(city->Limits().ContainsPoint(p)) {
			AddBuildingTile(p, st, *city, feet);
		}
		/*
		if(city->Limits().ContainsPoint(p)) {
			for(const auto& b: city->buildings) {
				Rect r(city->pos.x + b->xrel,
				       city->pos.y + b->yrel,
				       b->W()+1, b->H()+1); // +1 for shadow
				if(r.ContainsPoint(p)) {
					AddBuildingTile(p, st, *b, feet);
				}
			}
		}*/
	}
}


void 
CityEngine::AddBuildingTile(Point<int> p, std::queue<const Image*>& st, 
		const City& city, double feet) const
{
	// find building
	const Building* building = city.BuildingInPoint(p);
	if(!building)
		return;

	// if the building foot is below the character feet, we don't draw
	// the building in firt plane
	double bfoot = building->Y() + building->H();
	if(bfoot < (feet+1.5) && feet != 0.0)
		return;

	// find tiles
	Tiles tile = {
		city.Layout(p + Point<int>(-1, -1)),
		city.Layout(p + Point<int>( 0, -1)),
		city.Layout(p + Point<int>( 1, -1)),
		city.Layout(p + Point<int>(-1,  0)),
		city.Layout(p + Point<int>( 0,  0)),
		city.Layout(p + Point<int>( 1,  0)),
		city.Layout(p + Point<int>(-1,  1)),
		city.Layout(p + Point<int>( 0,  1)),
		city.Layout(p + Point<int>( 1,  1))
	};

	// draw stuff
	AddBackWall(st, tile);
	AddWall(st, tile);
	AddDoorFrames(st, tile);
}

void 
CityEngine::AddBackWall(std::queue<const Image*>& st, const Tiles& tile) const
{
	if((tile.c == "w1" || tile.c == "w4" || tile.c == "w7") 
	&& tile.w == "w2") {
		st.push(res["house_n"]);
	}
	if((tile.c == "w1" || tile.c == "w4" || tile.c == "w7") 
	&& tile.w == "w5") {
		st.push(res["house_c"]);
	}
	if((tile.c == "w1" || tile.c == "w4" || tile.c == "w7") 
	&& tile.w == "w8") {
		st.push(res["house_s"]);
	}
	if((tile.c == "w3" || tile.c == "w6" || tile.c == "w9") 
	&& tile.e == "w2") {
		st.push(res["house_n"]);
	}
	if((tile.c == "w3" || tile.c == "w6" || tile.c == "w9") 
	&& tile.e == "w5") {
		st.push(res["house_c"]);
	}
	if((tile.c == "w3" || tile.c == "w6" || tile.c == "w9") 
	&& tile.e == "w8") {
		st.push(res["house_s"]);
	}
}


void 
CityEngine::AddWall(std::queue<const Image*>& st, const Tiles& tile) const
{
	// draw walls
	if(tile.c == "w1") {
		st.push(res["house_nw"]);
	} else if(tile.c == "w2") {
		st.push(res["house_n"]);
	} else if(tile.c == "w3") {
		st.push(res["house_ne"]);
	} else if(tile.c == "w4") {
		st.push(res["house_w"]);
	} else if(tile.c == "w5") {
		st.push(res["house_c"]);
	} else if(tile.c == "w6") {
		st.push(res["house_e"]);
	} else if(tile.c == "w7") {
		st.push(res["house_sw"]);
	} else if(tile.c == "w8") {
		st.push(res["house_s"]);
	} else if(tile.c == "w9") {
		st.push(res["house_se"]);
	} else if(tile.c == "d1") {
		st.push(res["house_door_a_1"]);
	} else if(tile.c == "d2") {
		st.push(res["house_s"]);
		st.push(res["house_door_a_2"]);
		st.push(res["house_stairs_1"]);
	}

	// stairs
	if(tile.n == "d2") {
		st.push(res["house_stairs_2"]);
	}
}


void 
CityEngine::AddDoorFrames(std::queue<const Image*>& st, const Tiles& tile) const
{
	if(tile.se == "d1")
		st.push(res["house_door_frame_nw"]);
	if(tile.s == "d1")
		st.push(res["house_door_frame_n"]);
	if(tile.sw == "d1")
		st.push(res["house_door_frame_ne"]);
	if(tile.e == "d1")
		st.push(res["house_door_frame_w"]);
	if(tile.w == "d1")
		st.push(res["house_door_frame_e"]);
	if(tile.e == "d2")
		st.push(res["house_door_frame_sw"]);
	if(tile.w == "d2")
		st.push(res["house_door_frame_se"]);
}