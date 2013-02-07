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
CityEngine::AddBuildings(Tile p, ImgQueue& st, double feet) const
{
	for(const auto& city: world.map().cities()) {
		if(city->Limits().ContainsPoint(p)) {
			AddBuildingTile(p, st, *city, feet);
		}
	}
}


void 
CityEngine::AddBuildingTile(Tile p, ImgQueue& st, const City& city, 
		double feet) const
{
	// find building
	const Building* building = city.BuildingInPoint(p);
	if(!building)
		return;

	// if the building foot is below the character feet, we don't draw
	// the building in firt plane
	double bfoot = building->Y() + building->HeightAt(p.x);
	if(bfoot < (feet+1.5) && feet != 0.0)
		return;

	// find tiles
	Tiles tile = {
		city.Layout(p + Tile(-1, -1)),
		city.Layout(p + Tile( 0, -1)),
		city.Layout(p + Tile( 1, -1)),
		city.Layout(p + Tile(-1,  0)),
		city.Layout(p + Tile( 0,  0)),
		city.Layout(p + Tile( 1,  0)),
		city.Layout(p + Tile(-1,  1)),
		city.Layout(p + Tile( 0,  1)),
		city.Layout(p + Tile( 1,  1))
	};

	// draw stuff
	AddShadows(st, tile);
	AddBackWall(st, tile);
	AddWall(st, tile, *building);
	AddWindows(st, tile);
	AddDoorFrames(st, tile);
	AddRoof(st, tile);
}

void 
CityEngine::AddBackWall(ImgQueue& st, const Tiles& tile) const
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
CityEngine::AddWall(ImgQueue& st, const Tiles& tile, const Building& b) const
{
	// draw walls
	if(tile.c == "w1") {
		st.push(res["house_nw"]);
	} else if(tile.c == "w2") {
		st.push(res["house_n"]);
	} else if(tile.c == "w3") {
		st.push(res["house_ne"]);
		st.push(res["shadow_house_nw"]);
	} else if(tile.c == "w4") {
		st.push(res["house_w"]);
	} else if(tile.c == "w5") {
		st.push(res["house_c"]);
	} else if(tile.c == "w6") {
		st.push(res["house_e"]);
		st.push(res["shadow_house_nw"]);
	} else if(tile.c == "w7") {
		st.push(res["house_sw"]);
	} else if(tile.c == "w8") {
		st.push(res["house_s"]);
	} else if(tile.c == "w9") {
		st.push(res["house_se"]);
		st.push(res["shadow_house_sw"]);
	} else if(tile.c == "dn" || tile.c == "dN") {
		st.push(res["house_door_a_1"]);
	} else if(tile.c == "ds") {
		st.push(res["house_s"]);
		st.push(res["house_door_a_2"]);
		st.push(res["house_stairs_1"]);
	}

	// sign
	if(tile.c == "ws") {
		st.push(res["house_c"]);
		string s = b.Image().sign();
		if(s != "") {
			st.push(res["sign_" + s]);
		}
	}

	// stairs
	if(tile.n == "ds") {
		st.push(res["house_stairs_2"]);
	}
	if(tile.w == "ds") {
		st.push(res["shadow_stairs_n"]);
	}
	if(tile.nw == "ds") {
		st.push(res["shadow_stairs_s"]);
	}
}


void 
CityEngine::AddShadows(ImgQueue& st, const Tiles& tile) const
{
	if(tile.c != "  ") {
		return;
	}

	if(tile.w == "w9") {
		st.push(res["shadow_house_se"]);
	} else if(tile.w == "w6" || tile.w == "w3") {
		st.push(res["shadow_house_ne"]);
	}

	if(tile.w == "R9") {
		st.push(res["shadow_house_ne"]);
		st.push(res["shadow_roof_s"]);
	} else if(tile.w == "R6") {
		st.push(res["shadow_roof_n"]);
	}
}


void 
CityEngine::AddWindows(ImgQueue& st, const Tiles& tile) const
{
	if(tile.c == "Ww" || tile.c == "WW") {
		st.push(res["house_c"]);
		st.push(res["house_window_c"]);
	} else if(tile.s == "Ww") {
		st.push(res["house_window_n"]);
	} else if(tile.s == "WW") {
		st.push(res["house_window_decor"]);
	} else if(tile.n == "Ww" || tile.n == "WW") {
		st.push(res["house_window_s"]);
	}
}


void 
CityEngine::AddDoorFrames(ImgQueue& st, const Tiles& tile) const
{
	if(tile.se == "dn" || tile.se == "dN") {
		st.push(res["house_door_frame_nw"]);
	}
	if(tile.s == "dn") {
		st.push(res["house_door_frame_n"]);
	} else if(tile.s == "dN") {
		st.push(res["house_door_decor"]);
	}
	if(tile.sw == "dn" || tile.sw == "dN") {
		st.push(res["house_door_frame_ne"]);
	}
	if(tile.e == "dn" || tile.e == "dN") {
		st.push(res["house_door_frame_w"]);
	}
	if(tile.w == "dn" || tile.w == "dN") {
		st.push(res["house_door_frame_e"]);
	}
	if(tile.e == "ds") {
		st.push(res["house_door_frame_sw"]);
	}
	if(tile.w == "ds") {
		st.push(res["house_door_frame_se"]);
	}
}


void 
CityEngine::AddRoof(ImgQueue& st, const Tiles& tile) const
{
	// roof corners
	if(tile.c == "R1") { 
		st.push(res["house_roof_corner_nw"]);
	} else if(tile.c == "R2") { 
		st.push(res["house_roof_corner_n"]);
	} else if(tile.c == "R3") { 
		st.push(res["house_roof_corner_ne"]);
	} else if(tile.c == "R4") { 
		st.push(res["house_roof_corner_w"]);
	} else if(tile.c == "R5") { 
		st.push(res["house_roof_corner_c"]);
	} else if(tile.c == "R6") { 
		st.push(res["house_roof_corner_e"]);
	} else if(tile.c == "R7") { 
		st.push(res["house_roof_corner_sw"]);
	} else if(tile.c == "R8") { 
		st.push(res["house_roof_corner_s"]);
	} else if(tile.c == "R9") { 
		st.push(res["house_roof_corner_se"]);
	}

	// roofs
	if(tile.c == "r1") { 
		st.push(res["house_roof_nw"]);
	} else if(tile.c == "r2") { 
		st.push(res["house_roof_n"]);
	} else if(tile.c == "r3") { 
		st.push(res["house_roof_ne"]);
	} else if(tile.c == "r4") { 
		st.push(res["house_roof_w"]);
	} else if(tile.c == "r5") { 
		st.push(res["house_roof_c"]);
	} else if(tile.c == "r6") { 
		st.push(res["house_roof_e"]);
	} else if(tile.c == "r7") { 
		st.push(res["house_roof_sw"]);
	} else if(tile.c == "r8") { 
		st.push(res["house_roof_s"]);
	} else if(tile.c == "r9") { 
		st.push(res["house_roof_se"]);
	} else if(tile.c == "RL") { 
		st.push(res["house_roof_inner_left"]);
	} else if(tile.c == "RR") { 
		st.push(res["house_roof_inner_right"]);
	}

	// chimneys
	if(tile.c == "C6") {
		st.push(res["house_roof_e"]);
		st.push(res["house_chimney_s"]);
	} else if(tile.s == "C6") {
		st.push(res["house_chimney_n"]);
	}
	if(tile.c == "C5") {
		st.push(res["house_roof_c"]);
		st.push(res["house_chimney_s"]);
	} else if(tile.s == "C5") {
		st.push(res["house_chimney_n"]);
	}
		
}
