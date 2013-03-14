#include "defines.h"

#include <cstdlib>

struct LayerType {
	int layer;
	vector<string> tiles;
};
static const map<char, LayerType> layers = {
	{ 'T', { 1, { "grass_c", "water_c" } } },
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
	int x = 0, y = 0;
	try {
		for(auto const& tile: layers.at(option_).tiles) {
			ui->Scene().DrawImage(tile,
					ui->WindowZoomW() - ((3-x) * 24),
					8 + (y * 24));
			x++;
			if(x == 3) {
				x = 0; y++;
			}
		}
	} catch(out_of_range& e) { 
		abort();
	}
}
