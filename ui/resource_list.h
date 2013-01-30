#ifndef RESOURCE_LIST_H
#define RESOURCE_LIST_H

#include <vector>
#include <string>
#include "util/rect.h"

using namespace std;

// basic 9 tiles
static const vector<string> basic_9tiles { 
	"_nw", "_n", "_ne", "_w", "_c", "_e", "_sw", "_s", "_se"
};

// single images
static const vector<Rect> terrain_r = {
	Rect( 0, 160, 32, 32), Rect(32, 160, 32, 32), Rect(64, 160, 32, 32),
	Rect(32,   0, 32, 32), Rect(64,   0, 32, 32), Rect(32,  32, 32, 32), 
	Rect(64,  32, 32, 32), Rect( 0,  64, 32, 32), Rect(32,  64, 32, 32),
	Rect(64,  64, 32, 32), Rect( 0,  96, 32, 32), Rect(32,  96, 32, 32),
	Rect(64,  96, 32, 32), Rect( 0, 128, 32, 32), Rect(32, 128, 32, 32),
	Rect(64, 128, 32, 32), Rect( 0,   0, 32, 32), Rect(32,   0, 32, 32),
};

static const vector<string> terrain_sfx { 
	"_0", "_1", "_2",
	"_ic_nw", "_ic_ne", "_ic_sw", 
	"_ic_se", "_ec_nw", "_ec_n", 
	"_ec_ne", "_ec_w", "_c", 
	"_ec_e", "_ec_sw", "_ec_s",
	"_ec_se", "_s_1", "_s_2"
};

// minimap tiles
static const vector<Rect> mm_r {
	Rect(353, 128, 60, 67), Rect(413, 128, 66, 67), Rect(479, 128, 64, 67),
	Rect(353, 195, 60, 59), Rect(413, 195, 66, 59), Rect(479, 195, 64, 59),
	Rect(353, 254, 60, 65), Rect(413, 254, 66, 65), Rect(479, 254, 64, 65),
};

// character
static const int dx = 17, dy = 15, dw=48-dx, dh=60-dy;
static constexpr Rect cr(int x, int y) { return Rect(64*x+dx, 64*y+dy, dw, dh); }
static const vector<Rect> char_rects {
	cr(0,0),cr(1,0),cr(2,0),cr(3,0),cr(4,0),cr(5,0),cr(6,0),cr(7,0),cr(8,0), 
	cr(0,1),cr(1,1),cr(2,1),cr(3,1),cr(4,1),cr(5,1),cr(6,1),cr(7,1),cr(8,1), 
	cr(0,2),cr(1,2),cr(2,2),cr(3,2),cr(4,2),cr(5,2),cr(6,2),cr(7,2),cr(8,2),
	cr(0,3),cr(1,3),cr(2,3),cr(3,3),cr(4,3),cr(5,3),cr(6,3),cr(7,3),cr(8,3)
};
static const vector<string> char_sfx {
	"_n_0", "_n_1", "_n_2", "_n_3", "_n_4", "_n_5", "_n_6", "_n_7", "_n_8", 
	"_w_0", "_w_1", "_w_2", "_w_3", "_w_4", "_w_5", "_w_6", "_w_7", "_w_8",
	"_s_0", "_s_1", "_s_2", "_s_3", "_s_4", "_s_5", "_s_6", "_s_7", "_s_8", 
	"_e_0", "_e_1", "_e_2", "_e_3", "_e_4", "_e_5", "_e_6", "_e_7", "_e_8"
};

// trunk
static constexpr Rect tr(int x, int y) { return Rect(x*32, y*32, 32, 32); }
static const vector<Rect> trunk_rect {
	tr(0,0),tr(1,0),tr(2,0),tr(3,0),tr(4,0),tr(5,0),
        tr(0,1),tr(1,1),tr(2,1),tr(3,1),tr(4,1),tr(5,1),
        tr(0,2),tr(1,2),tr(2,2),tr(3,2),tr(4,2),tr(5,2),
        tr(0,3),tr(1,3),tr(2,3),tr(3,3),tr(4,3),tr(5,3)
};
static const vector<string> trunk_sfx {
	"_1_nw", "_1_n", "_1_ne", "_2_nw", "_2_n", "_2_ne", 
	"_1_w",  "_1_c", "_1_e",  "_2_w",  "_2_c", "_2_e", 
	"_1_sw", "_1_s", "_1_se", "_2_sw", "_2_s", "_2_se"
};

// treetop
static const vector<Rect> treetop_rect {
	tr(0,0),tr(1,0),tr(2,0),tr(3,0),tr(4,0),tr(5,0),
        tr(0,1),tr(1,1),tr(2,1),tr(3,1),tr(4,1),tr(5,1),
        tr(0,2),tr(1,2),tr(2,2),tr(3,2),tr(4,2),tr(5,2),
        tr(0,3),tr(1,3),tr(2,3),tr(3,3),tr(4,3),tr(5,3),
	tr(0,4),tr(1,4),tr(2,4),tr(3,4),tr(4,4),tr(5,4),
        tr(0,5),tr(1,5),tr(2,5),tr(3,5),tr(4,5),tr(5,5),
        tr(0,6),tr(1,6),tr(2,6),tr(3,6),tr(4,6),tr(5,6),
        tr(0,7),tr(1,7),tr(2,7),tr(3,7),tr(4,7),tr(5,7)
};
static const vector<string> treetop_sfx {
	"_1_a_nw", "_1_a_n", "_1_a_ne", "_1_b_nw", "_1_b_n", "_1_b_ne", 
	"_1_a_w",  "_1_a_c", "_1_a_e",  "_1_b_w",  "_1_b_c", "_1_b_e", 
	"_1_a_sw", "_1_a_s", "_1_a_se", "_1_b_sw", "_1_b_s", "_1_b_se",
	"_2_a_nw", "_2_a_n", "_2_a_ne", "_2_b_nw", "_2_b_n", "_2_b_ne", 
	"_2_a_w",  "_2_a_c", "_2_a_e",  "_2_b_w",  "_2_b_c", "_2_b_e", 
	"_2_a_sw", "_2_a_s", "_2_a_se", "_2_b_sw", "_2_b_s", "_2_b_se"
};

// trunk and treetop - full
static const vector<Rect> trunkfull_rect {
	Rect(0, 0, 96, 96), Rect(96, 0, 96, 96)
};
static const vector<string> trunkfull_sfx { "_1", "_2" };
static const vector<Rect> treetopfull_rect {
	Rect(0,  0, 96, 96), Rect(96,  0, 96, 96),
	Rect(0, 96, 96, 96), Rect(96, 96, 96, 96),
};
static const vector<string> treetopfull_sfx { "_1_a", "_1_b", "_2_a", "_2_b" };

// house
static const vector<Rect> house_rect {
	tr(0,0),tr(1,0),tr(2,0), 		// walls
        tr(0,1),tr(1,1),tr(2,1),
        tr(0,2),tr(1,2),tr(2,2),
	tr(3,0),tr(3,1),tr(5,0),tr(5,1), 	// doors
	tr(4,0),tr(4,1),			// stairs
	Rect(232,90,48,54),                     // door frame
};
static const vector<string> house_sfx {
	"_nw", "_n", "_ne", "_w", "_c", "_e", "_sw", "_s", "_se",
	"_door_a_1", "_door_a_2", "_door_b_1", "door_b_2",
	"_stairs_1", "_stairs_2",
	"_door_frame",
};

// resource file list
static const struct {
	const string name;
	const string filename;
	const vector<Rect> r;
	const vector<string> suffix;
} reslist[] = {

	// terrains
	{ "grass",     "grass.png",     terrain_r, terrain_sfx },
	{ "grassalt",  "grassalt.png",  terrain_r, terrain_sfx },
	{ "dirt",      "dirt.png",      terrain_r, terrain_sfx },
	{ "earth",     "dirt2.png",     terrain_r, terrain_sfx },
	{ "lava",      "lava.png",      terrain_r, terrain_sfx },
	{ "lavarock",  "lavarock.png",  terrain_r, terrain_sfx },
	{ "water",     "water.png",     terrain_r, terrain_sfx },
	{ "watergrass","watergrass.png",terrain_r, terrain_sfx },
	{ "snow",      "snow.png",      terrain_r, terrain_sfx },

	// trees - partial
	{ "trunk",   "trunk.png",       trunk_rect,   trunk_sfx   },
	{ "trunksh", "trunkshadow.png", trunk_rect,   trunk_sfx   },
	{ "treetop", "treetop.png",     treetop_rect, treetop_sfx },

	// trees - full
	{ "trunkfull",   "trunk.png",       trunkfull_rect,   trunkfull_sfx   },
	{ "trunkshfull", "trunkshadow.png", trunkfull_rect,   trunkfull_sfx   },
	{ "treetopfull", "treetop.png",     treetopfull_rect, treetopfull_sfx },

	// map
	{ "mm", "scrollsandblocks.png", mm_r, basic_9tiles },

	// characters
	{ "male",  "male_walkcycle.png", char_rects, char_sfx },
	{ "pants", "male_pants.png",     char_rects, char_sfx },

	// house
	{ "house", "house.png", house_rect, house_sfx },

	// terminal
	{ "terminal", "terminal.png", {}, {} },

	// fonts
	{ "termfont", "Glass_TTY_VT220.ttf", { Rect(20) }, { "_20" } },
};

#endif
