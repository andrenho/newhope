#ifndef UI_TERRAINSURFACE_H
#define UI_TERRAINSURFACE_H

#include <climits>
#include <map>
#include <queue>
#include <set>
#include <vector>

#include "util/point.h"
#include "util/rect.h"
#include "util/defines.h"
#include "world/world.h"

class Building;
class CityEngine;
class GraphicLibrary;
class Image;
class Resources;
class World;

typedef std::queue<const Image*> ImgQueue;

class TerrainSurface {
public:
	TerrainSurface(const World& world, const GraphicLibrary& video,
			const Resources& res);
	~TerrainSurface();

	void RedrawImg(std::vector<Rect>& rects);
	void Resize(int scr_w, int scr_h);
	void AreasToRedraw(std::vector<Rect>& rects);
	void SetTopLeft(Tile p);
	void AddFirstPlane(Tile p, ImgQueue& st, double feet=0.0) const;

	inline const Image& image() const { return *image_; }

private:
	void Redraw();
	void DrawTile(Tile p);

	// back plane
	const Image* TileSurface(Tile p);
	void BuildTile(Tile p, ImgQueue& st);
	void BuildTileBorders(Tile p, TerrainType t, ImgQueue& st);
	void BuildBorder(TerrainType t, uint8_t bs, ImgQueue& st);
	void AddTreeShadows(Tile p, ImgQueue& st) const;

	// first plane
	void AddTrees(Tile p, ImgQueue& st, double feet) const;
	
	std::map<ImgQueue, const Image*> imagehash;

	Image* image_;

	const World& world;
	const GraphicLibrary& video;
	const Resources& res;
	int x, y, w, h;
	std::set<Tile> tiles_to_redraw;
	const CityEngine* city_engine;

	DISALLOW_COPY_AND_ASSIGN(TerrainSurface);
};

#endif
