#include "ui/terrainsurface.h"

#include <cstdint>
#include <algorithm>
#include <bitset>
#include <sstream>
#include <vector>
using namespace std;

#include "SDL.h"

#include "libs/graphiclibrary.h"
#include "libs/image.h"
#include "ui/cityengine.h"
#include "ui/resource.h"
#include "util/logger.h"
#include "world/building.h"
#include "world/city.h"
#include "world/mapbuild.h"

TerrainSurface::TerrainSurface(const World& world, const GraphicLibrary& video,
		const Resources& res) :
	Img(nullptr), world(world), video(video), res(res), 
	x(-10000), y(-10000), w(0), h(0), 
	city_engine(new CityEngine(world, video, res))
{ 
}


TerrainSurface::~TerrainSurface()
{
	for(const auto& image: imagehash) {
		delete image.second;
	}
	if(Img) {
		delete Img;
	}
	delete city_engine;
}


void
TerrainSurface::RedrawImg(vector<Rect>& rects)
{
	uint32_t t(SDL_GetTicks());

	for(const auto& tile: tiles_to_redraw) {
		rects.push_back(Rect(tile.x, tile.y, TileSize, TileSize));
		DrawTile(tile);
	}

	if(!tiles_to_redraw.empty()) {
		logger.DebugFrame("Frame redraw: %d tiles, %d ms", 
				tiles_to_redraw.size(), SDL_GetTicks() - t);
	}
	tiles_to_redraw.clear();
}


void 
TerrainSurface::Resize(int scr_w, int scr_h)
{
	if(Img) {
		delete Img;
	}
	Img = video.CreateImage(scr_w + (TileSize + (scr_w % TileSize)),
			        scr_h + (TileSize + (scr_h % TileSize)));
	this->w = Img->w / TileSize;
	this->h = Img->h / TileSize;

	tiles_to_redraw.clear();
	Redraw();

	SetTopLeft({0, 0});

	logger.Debug("window resize resquested: %d %d", w, h);
	logger.Debug("trsurf resize: %d %d (%d %d)", Img->w, Img->h, w, h);
}


void 
TerrainSurface::AreasToRedraw(vector<Rect>& rects)
{
}


void 
TerrainSurface::SetTopLeft(Point<int> p)
{
	tiles_to_redraw.clear();

	if(this->x == p.x && this->y == p.y) {
		return;
	}

	if(abs(this->x - p.x)*TileSize > video.Window->w
	|| abs(this->y - p.y)*TileSize > video.Window->h) {
		this->x = p.x;
		this->y = p.y;
		Redraw();
	} else {
		int nx, ny;

		Rect r((this->x - p.x)*TileSize, (this->y - p.y)*TileSize);
		Img->Blit(*Img, r);

		int tsx = this->x, 
		    tsy = this->y;

		this->x = p.x;
		this->y = p.y;

		if(p.x > tsx)
		{
			for(nx=(this->w + this->x - (p.x - tsx)); 
					nx < (this->w + this->x); nx++) {
				for(ny=this->y; ny < (this->h + this->y); ny++) {
					tiles_to_redraw.insert(Point<int>(nx, ny));
				}
			}
		} else if(p.x < tsx) {
			for(nx=this->x; nx < (this->x + tsx - p.x); nx++) {
				for(ny=this->y; ny < (this->h + this->y); ny++) {
					tiles_to_redraw.insert(Point<int>(nx, ny));
				}
			}
		}
		if(p.y > tsy) {
			for(ny=(this->h + this->y - (p.y - tsy)); 
					ny < (this->h + this->y); ny++) {
				for(nx=this->x; nx < (this->w + this->x); nx++) {
					tiles_to_redraw.insert(Point<int>(nx, ny));
				}
			}
		}
		else if(p.y < tsy) {
			for(ny=this->y; ny < (this->y + tsy - p.y); ny++) {
				for(nx=this->x; nx < (this->w + this->x); nx++) {
					tiles_to_redraw.insert(Point<int>(nx, ny));
				}
			}
		}
	}
}


void
TerrainSurface::Redraw()
{
	for(int x(this->x); x<(this->x + this->w); x++)
		for(int y(this->y); y<(this->y + this->h); y++)
			tiles_to_redraw.insert(Point<int>(x, y));
}


void 
TerrainSurface::DrawTile(Point<int> p)
{
	Rect r((p.x - this->x) * TileSize, 
	       (p.y - this->y) * TileSize, 
	       TileSize, TileSize);

	const Image* img = TileSurface(p);
	img->Blit(*Img, r);
}


const Image* 
TerrainSurface::TileSurface(Point<int> p)
{
	// build queue
	queue<const Image*> st;
	BuildTile(p, st);

	// lookup in the hash
	if(imagehash.find(st) == imagehash.end()) {
		// not found, create image
		Image* image(video.CreateImage(TileSize, TileSize));
		imagehash[st] = image;
		while(!st.empty()) {
			st.front()->Blit(*image);
			st.pop();
		}
		return image;
	}
	return imagehash[st];
}


static map<TerrainType, string> basic {
	{ t_GRASS,    "grass"    },
	{ t_WATER,    "water"    },
	{ t_DIRT,     "dirt"     },
	{ t_EARTH,    "earth"    },
	{ t_LAVAROCK, "lavarock" },
	{ t_LAVA,     "lava"     },
	{ t_SNOW,     "snow"     },

	/* TODO */
	{ t_HOTFOREST, "grass" },
	{ t_TUNDRA, "earth" },
	{ t_COLDFOREST, "grass" },

	{ t_ROAD, "lavarock" },
	{ t_RIVER, "water" }
};


void 
TerrainSurface::BuildTile(Point<int> p, queue<const Image*>& st)
{
	// basic terrain
	TerrainType terrain(world.Terrain(p));
	string basic_terrain(basic[terrain]);
	int special(world.Special(p));
	if(special <= 2) {
		stringstream s;
		s << basic_terrain << "_" << special;
		st.push(res[s.str()]);
	} else {
		st.push(res[basic_terrain + "_c"]);
	}

	// borders
	BuildTileBorders(p, terrain, st);

	// add tree shadows
	AddTreeShadows(p, st);

	// add first plane images
	AddFirstPlane(p, st);
}


void
TerrainSurface::BuildTileBorders(Point<int> p, TerrainType t, 
		queue<const Image*>& st)
{
	// find terrains around
	TerrainType around[8] {
		world.Terrain(Point<int>(p.x-1, p.y-1)),
		world.Terrain(Point<int>(p.x  , p.y-1)),
		world.Terrain(Point<int>(p.x+1, p.y-1)),
		world.Terrain(Point<int>(p.x-1, p.y  )),
		world.Terrain(Point<int>(p.x+1, p.y  )),
		world.Terrain(Point<int>(p.x-1, p.y+1)),
		world.Terrain(Point<int>(p.x  , p.y+1)),
		world.Terrain(Point<int>(p.x+1, p.y+1))
	};
	
	// compact list and order by importance
	vector<TerrainType> terrains;
	for(int i(0); i<8; i++) {
		if(find(terrains.begin(), terrains.end(), around[i]) ==
				terrains.end()) {
			terrains.push_back(around[i]);
		}
	}
	sort(terrains.begin(), terrains.end());

	// find borders
	for(const auto& terrain: terrains) {
		if(terrain <= t) {
			continue;
		}
		uint8_t bs(0);
		for(int i(0); i<8; i++) {
			bs |= ((around[i] == terrain) << i);
		}
		BuildBorder(terrain, bs, st);
	}
}


void
TerrainSurface::BuildBorder(TerrainType t, uint8_t bs, queue<const Image*>& st)
{
	struct { int nw, n, ne, w, e, sw, s, se; } b = {
		(bs & 0b00000001), (bs & 0b00000010), (bs & 0b00000100),
		(bs & 0b00001000), 		      (bs & 0b00010000),
 		(bs & 0b00100000), (bs & 0b01000000), (bs & 0b10000000) };

	// find the most t tile around this one
	if(b.s && b.w)
		st.push(res[basic[t] + "_ic_sw"]);
	else if(b.s && b.e)
		st.push(res[basic[t] + "_ic_se"]);
	else if(b.n && b.w)
		st.push(res[basic[t] + "_ic_nw"]);
	else if(b.n && b.e)
		st.push(res[basic[t] + "_ic_ne"]);
	else if(b.n || b.s || b.w || b.e)
	{
		if(b.n)
			st.push(res[basic[t] + "_ec_s"]);
		if(b.s)
			st.push(res[basic[t] + "_ec_n"]);
		if(b.e)
			st.push(res[basic[t] + "_ec_w"]);
		if(b.w)
			st.push(res[basic[t] + "_ec_e"]);
	}
	else if(b.se || b.sw)
	{
		if(b.se)
			st.push(res[basic[t] + "_ec_nw"]);
		if(b.sw)
			st.push(res[basic[t] + "_ec_ne"]);
	}
	else if(b.ne || b.nw)
	{
		if(b.ne)
			st.push(res[basic[t] + "_ec_sw"]);
		if(b.nw)
			st.push(res[basic[t] + "_ec_se"]);
	}
}


void 
TerrainSurface::AddTreeShadows(Point<int> p, std::queue<const Image*>& st) const
{
	// sides
	static vector<string> sfx = {
		"se", "s", "sw", "e", "c", "w", "ne", "n", "nw"
	};
	
	// find terrains around
	Point<int> around[9] {
		{p.x-1, p.y-1}, {p.x  , p.y-1}, {p.x+1, p.y-1},
		{p.x-1, p.y  }, {p.x,   p.y  }, {p.x+1, p.y  },
		{p.x-1, p.y+1}, {p.x  , p.y+1}, {p.x+1, p.y+1} };

	// find trunk images
	for(int i(0); i<9; i++) {
		string treecode;
		TreeType t(world.Tree(around[i]));
		if(t == TreeType::TREE_ROUND) {
			treecode = "1";
		} else if(t == TreeType::TREE_POINTY) {
			treecode = "2";
		} else {
			continue;
		}
		st.push(res["trunksh_" + treecode + "_" + sfx[i]]);
	}
}


void 
TerrainSurface::AddFirstPlane(Point<int> p, std::queue<const Image*>& st,
		double feet) const
{
	AddTrees(p, st, feet);
	AddBuildings(p, st, feet);
}


void 
TerrainSurface::AddTrees(Point<int> p, queue<const Image*>& st, 
		double feet) const
{
	// sides
	static vector<string> sfx = {
		"se", "s", "sw", "e", "c", "w", "ne", "n", "nw"
	};

	// find terrains around
	Point<int> around[9] {
		{p.x-1, p.y-1}, {p.x  , p.y-1}, {p.x+1, p.y-1},
		{p.x-1, p.y  }, {p.x,   p.y  }, {p.x+1, p.y  },
		{p.x-1, p.y+1}, {p.x  , p.y+1}, {p.x+1, p.y+1} };
	Point<int> around_treetop[9] {
		{p.x-1, p.y+1}, {p.x  , p.y+1}, {p.x+1, p.y+1},
		{p.x-1, p.y+2}, {p.x,   p.y+2}, {p.x+1, p.y+2},
		{p.x-1, p.y+3}, {p.x  , p.y+3}, {p.x+1, p.y+3} };

	// find trunk images
	for(int i(0); i<9; i++) {
		if(around[i].y-0.5 < feet) {
			continue;
		}
		string treecode;
		TreeType t(world.Tree(around[i]));
		if(t == TreeType::TREE_ROUND) {
			treecode = "1";
		} else if(t == TreeType::TREE_POINTY) {
			treecode = "2";
		} else {
			continue;
		}
		st.push(res["trunk_" + treecode + "_" + sfx[i]]);
	}

	// find treetop images
	for(int i(0); i<9 ;i++) {
		string treecode;
		string cl(world.TreeSmall(around_treetop[i]) ? "b" : "a");
		TreeType t(world.Tree(around_treetop[i]));
		if(t == TreeType::TREE_ROUND) {
			treecode = "1";
		} else if(t == TreeType::TREE_POINTY) {
			treecode = "2";
		} else {
			continue;
		}
		st.push(res["treetop_" + treecode + "_" + cl + "_" + sfx[i]]);
	}
}


void 
TerrainSurface::AddBuildings(Point<int> p, std::queue<const Image*>& st, 
		double feet) const
{
	for(const auto& city: world.map->cities) {
		if(city->Limits().ContainsPoint(p)) {
			for(const auto& b: city->buildings) {
				Rect r(city->pos.x + b->xrel,
				       city->pos.y + b->yrel,
				       b->W(), b->H());
				if(r.ContainsPoint(p)) {
					AddBuildingTile(p, st, feet);
				}
			}
		}
	}
}


void 
TerrainSurface::AddBuildingTile(Point<int> p, std::queue<const Image*>& st, 
		double feet) const
{
	if(p.y-0.5 < feet) {
		return;
	}
	st.push(res["house_c"]);
}
