#include "defines.h"

void
LayerTerrain::Render() const
{
	ui->Scene().setProportion(PROP_TILES);

	int x1, y1, x2, y2;
	ui->Scene().WindowTileBorders(x1, y1, x2, y2);
	for(int x=x1; x < x2; x++) {
		for(int y=y1; y < y2; y++) {
			DrawTile(x, y);
		}
	}
}

void
LayerTerrain::DrawTile(int x, int y) const
{
	vector<string> suffixes;
	TileSuffixes(x, y, suffixes);
	if(suffixes.empty()) {
		return;
	}

	for(auto const& suffix: suffixes) {
		ui->Scene().DrawImage(TerrainStr() + "_" + suffix, x, y);
	}
}


void
LayerTerrain::TileSuffixes(int x, int y, vector<string>& s) const
{
	Map const& m = game->Map();

	// central tile
	if(m.Terrain(x, y) == terrain_) {
		s.push_back("c");
		return;
	}

	// inner corners
	if(m.Terrain(x-1, y) == terrain_ && m.Terrain(x, y-1) == terrain_) {
		s.push_back("corner_nw");
	}
	if(m.Terrain(x+1, y) == terrain_ && m.Terrain(x, y-1) == terrain_) {
		s.push_back("corner_ne");
	}
	if(m.Terrain(x-1, y) == terrain_ && m.Terrain(x, y+1) == terrain_) {
		s.push_back("corner_sw");
	}
	if(m.Terrain(x+1, y) == terrain_ && m.Terrain(x, y+1) == terrain_) {
		s.push_back("corner_se");
	}

	// outer corners
	if(m.Terrain(x-1, y-1) == terrain_ 
	&& m.Terrain(x, y-1) != terrain_ && m.Terrain(x-1, y) != terrain_) {
		s.push_back("se");
	}
	if(m.Terrain(x+1, y-1) == terrain_ 
	&& m.Terrain(x, y-1) != terrain_ && m.Terrain(x+1, y) != terrain_) {
		s.push_back("sw");
	}
	if(m.Terrain(x-1, y+1) == terrain_ 
	&& m.Terrain(x, y+1) != terrain_ && m.Terrain(x-1, y) != terrain_) {
		s.push_back("ne");
	}
	if(m.Terrain(x+1, y+1) == terrain_ 
	&& m.Terrain(x, y+1) != terrain_ && m.Terrain(x+1, y) != terrain_) {
		s.push_back("nw");
	}

	// sides
	if(m.Terrain(x-1, y) == terrain_
	&& m.Terrain(x, y-1) != terrain_ && m.Terrain(x, y+1) != terrain_) {
		s.push_back("e");
	}
	if(m.Terrain(x+1, y) == terrain_
	&& m.Terrain(x, y-1) != terrain_ && m.Terrain(x, y+1) != terrain_) {
		s.push_back("w");
	}
	if(m.Terrain(x, y-1) == terrain_
	&& m.Terrain(x-1, y) != terrain_ && m.Terrain(x+1, y) != terrain_) {
		s.push_back("s");
	}
	if(m.Terrain(x, y+1) == terrain_
	&& m.Terrain(x-1, y) != terrain_ && m.Terrain(x+1, y) != terrain_) {
		s.push_back("n");
	}
}


string 
LayerTerrain::TerrainStr() const
{
	switch(terrain_)
	{
	case t_GRASS:  return "grass";
	case t_DESERT: return "desert";
	case t_DIRT:   return "dirt";
	case t_DIRT2:  return "dirt2";
	case t_WATER:  return "water";
	case t_SNOW:   return "snow";
	case t_LAVA:   return "lava";
	case t_TUNDRA: return "tundra";
	case t_LAVAROCK: return "lavarock";
	default: throw new ui_error("Invalid terrain");
	}
}
