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
		try {
			ui->Scene().DrawImage(TerrainStr() + "_" + suffix, x, y);
		} catch(out_of_range& e) {
			ui->Scene().DrawImage(suffix, x, y);
		}
	}
}


void
LayerTerrain::TileSuffixes(int x, int y, vector<string>& s) const
{
	Map const& m = game->Map();

	// central tile
	if(m.Terrain(Point<int>(x, y)) == terrain_) {
		s.push_back("c");

		int special = m.Special(Point<int>(x, y));
		if(special) {
			SpecialSuffix(special, s);
		}
		return;
	}

	// inner corners
	if(m.Terrain(Point<int>(x-1, y)) == terrain_ 
	&& m.Terrain(Point<int>(x, y-1)) == terrain_) {
		s.push_back("corner_nw");
	}
	if(m.Terrain(Point<int>(x+1, y)) == terrain_ 
	&& m.Terrain(Point<int>(x, y-1)) == terrain_) {
		s.push_back("corner_ne");
	}
	if(m.Terrain(Point<int>(x-1, y)) == terrain_ 
	&& m.Terrain(Point<int>(x, y+1)) == terrain_) {
		s.push_back("corner_sw");
	}
	if(m.Terrain(Point<int>(x+1, y)) == terrain_ 
	&& m.Terrain(Point<int>(x, y+1)) == terrain_) {
		s.push_back("corner_se");
	}

	// outer corners
	if(m.Terrain(Point<int>(x-1, y-1)) == terrain_ 
	&& m.Terrain(Point<int>(x, y-1)) != terrain_
	&& m.Terrain(Point<int>(x-1, y)) != terrain_) {
		s.push_back("se");
	}
	if(m.Terrain(Point<int>(x+1, y-1)) == terrain_ 
	&& m.Terrain(Point<int>(x, y-1)) != terrain_ 
	&& m.Terrain(Point<int>(x+1, y)) != terrain_) {
		s.push_back("sw");
	}
	if(m.Terrain(Point<int>(x-1, y+1)) == terrain_ 
	&& m.Terrain(Point<int>(x, y+1)) != terrain_ 
	&& m.Terrain(Point<int>(x-1, y)) != terrain_) {
		s.push_back("ne");
	}
	if(m.Terrain(Point<int>(x+1, y+1)) == terrain_ 
	&& m.Terrain(Point<int>(x, y+1)) != terrain_ 
	&& m.Terrain(Point<int>(x+1, y)) != terrain_) {
		s.push_back("nw");
	}

	// sides
	if(m.Terrain(Point<int>(x-1, y)) == terrain_
	&& m.Terrain(Point<int>(x, y-1)) != terrain_ 
	&& m.Terrain(Point<int>(x, y+1)) != terrain_) {
		s.push_back("e");
	}
	if(m.Terrain(Point<int>(x+1, y)) == terrain_
	&& m.Terrain(Point<int>(x, y-1)) != terrain_ 
	&& m.Terrain(Point<int>(x, y+1)) != terrain_) {
		s.push_back("w");
	}
	if(m.Terrain(Point<int>(x, y-1)) == terrain_
	&& m.Terrain(Point<int>(x-1, y)) != terrain_ 
	&& m.Terrain(Point<int>(x+1, y)) != terrain_) {
		s.push_back("s");
	}
	if(m.Terrain(Point<int>(x, y+1)) == terrain_
	&& m.Terrain(Point<int>(x-1, y)) != terrain_ 
	&& m.Terrain(Point<int>(x+1, y)) != terrain_) {
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


void 
LayerTerrain::SpecialSuffix(int special, vector<string>& s) const
{
	static map<Terrain, vector<string>> specials = {
		{ t_GRASS,    { "decor_flower_1", "decor_flower_2", "decor_flower_3", 
		                "decor_flower_4", "decor_rock_1", "decor_rock_2" } },
		{ t_DESERT,   { "decor_rock_3", "decor_skull", "decor_cactus", 
				"decor_rock_2", "" } },
		{ t_DIRT,     { "dirt_dif_1", "dirt_dif_2", "decor_skull", 
				"decor_cactus", "decor_rock_3" } },
		{ t_DIRT2,    { "dirt2_dif_1", "dirt2_dif_2", "decor_skull", 
				"decor_cactus", "decor_rock_3" } },
		{ t_WATER,    { "water_dif_1", "water_dif_2", "", "", "" } },
		{ t_SNOW,     { "decor_rock_1", "decor_rock_2", "decor_rock_3", 
				"snow_dif_1", "snow_dif_2" } },
		{ t_LAVA,     { "lava_dif_1", "lava_dif_2", "", "", "" } },
		{ t_TUNDRA,   { "tundra_dif_1", "tundra_dif_2", "decor_rock_1", 
				"decor_rock_2", "decor_rock_3" } },
		{ t_LAVAROCK, { "lavarock_dif_1", "lavarock_dif_2", 
				"decor_flower_4", "decor_flower_2", "" } },
	};

	try {
		string sp = specials[terrain_][special-1];
		if(sp != "") {
			s.push_back(sp);
		}
	} catch(out_of_range& e) { }
}
