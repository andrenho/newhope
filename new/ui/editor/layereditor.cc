#include "defines.h"

#include <cstdlib>

struct LayerType {
	int layer;
	vector<string> tiles;
};
static const map<char, LayerType> layers = {
	{ 'T', { 1, { "grass_c", "desert_c", "dirt_c", "dirt2_c", "snow_c",
		    "water_c", "lava_c", "tundra_c", "lavarock_c" } } },
	{ 'R', { 1, { "nothing", "tree_pine", "tree_conifer", "tree_broadleaf" } } },
};

void 
LayerEditor::Render() const
{
	// setup
	ui->Scene().setProportion(PROP_WINDOW);

	// draw black rectangle
	ui->Scene().DrawRectangle(ui->WindowZoomW() - 80, 0,
			ui->WindowZoomW(), ui->WindowZoomH(),
			Color { 0, 0, 0 });

	// draw tiles
	int x = 0, y = 0, i = 0;
	try {
		for(auto const& tile: layers.at(option_).tiles) {
			int px = ui->WindowZoomW() - ((3-x) * 24);
			int py = 8 + (y * 24);
			if(i == selected_) {
				ui->Scene().DrawRectangle(px-4, py-4, 
						px+20, py+20, { 255, 255, 255 });
				ui->Scene().DrawRectangle(px-3, py-3, 
						px+19, py+19, { 0, 0, 0 });
			}
			ui->Scene().DrawImage(tile, px, py, 16, 16);
			if(++x == 3) {
				x = 0; y++;
			}
			++i;
		}
	} catch(out_of_range& e) { 
		abort();
	}
}


void 
LayerEditor::setSelected(int s)
{
	int c = layers.at(option_).tiles.size();
	if(s < c) {
		selected_ = s;
	}
}


string 
LayerEditor::Selected() const
{
	return layers.at(option_).tiles[selected_];
}
