#include "defines.h"

#include <GL/gl.h>

void
Imageset::SetupImageMap()
{
	// terrains
	SetupImageTerrain("grass",    1,  1, 2);
	SetupImageTerrain("desert",   1,  6, 2);
	SetupImageTerrain("dirt",     1, 11, 2);
	SetupImageTerrain("dirt2",    1, 16, 2);
	SetupImageTerrain("snow",     1, 21, 2);
	SetupImageTerrain("water",    1,  1, 6);
	SetupImageTerrain("lava",     1,  6, 6);
	SetupImageTerrain("tundra",   1, 11, 6);
	SetupImageTerrain("lavarock", 1, 16, 6);

	// setup dialog
	SetupDialog("dialog", 0);

	// font
	SetupFont("font", 2, 8, 12);
}


Imageset::Imageset()
	: texture_(nullptr), c_texture(0)
{
	// load images
	static vector<string> images = { "dialog.png", "tiles.png", "font.png" };
	n_textures = images.size();
	texture_ = new GLuint[n_textures];
	for(int n=0; n<n_textures; n++) {
		texture_[n] = 0;
	}
	glGenTextures(n_textures, &texture_[c_texture]);
	
	for(string image: images) {
		LoadImage(image);
	}

	// texture parameters
	// setup images map
	SetupImageMap();
}


Imageset::~Imageset()
{
	hash_.clear();
	delete[] texture_;
}


void
Imageset::LoadImage(string filename)
{
	// load tiles
	Image* tileset = Image::LoadImage(filename);
	ImageRGB t = *((ImageIndexed*)tileset);

	glBindTexture(GL_TEXTURE_2D, texture_[c_texture]);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, t.W, t.H, 0, GL_RGBA, 
			GL_UNSIGNED_BYTE, t.Data());
	
	delete tileset;
	sizes_[c_texture] = { (float)t.W, (float)t.H };
	c_texture++;
}


void 
Imageset::SetupImageTerrain(string name, int idx, float x, float y)
{
	static const struct Corner { string suffix; int x, y; } corners[] = {
		{ "nw", -1, -1 },
		{ "n",   0, -1 },
		{ "ne",  1, -1 },
		{ "w",  -1,  0 },
		{ "c",   0,  0 },
		{ "e",   1,  0 },
		{ "sw", -1,  1 },
		{ "s",   0,  1 },
		{ "se",  1,  1 },
		{ "corner_nw",  2, -1 },
		{ "corner_ne",  3, -1 },
		{ "corner_sw",  2,  0 },
		{ "corner_se",  3,  0 },
		{ "dif_1", 2, 1 },
		{ "dif_2", 3, 1 },
	};

	for(auto const& corner: corners) {
		hash_[name + "_" + corner.suffix] = { idx,
			(x + corner.x) / sizes_[idx].w * 16,
			(y + corner.y) / sizes_[idx].h * 16,
			1.0f / sizes_[idx].w * 16,
			1.0f / sizes_[idx].h * 16,
			16, 16 };
	}
}


void 
Imageset::SetupFont(string name, int idx, int w, int h)
{
	for(int c = 0; c <= 255; c++) {
		hash_[(name + "_").append(1, (char)c)] = { idx,
			(c % 16) * w / sizes_[idx].w,
			(c / 16) / (sizes_[idx].h / h),
			1.0f / 16.0f,
			1.0f / 16.0f,
			8, 12 };
	}
}


void
Imageset::SetupDialog(string name, int idx) 
{
	int x=1, y=1;
	static const struct Corner { string suffix; int x, y; } corners[] = {
		{ "nw", -1, -1 },
		{ "n",   0, -1 },
		{ "ne",  1, -1 },
		{ "w",  -1,  0 },
		{ "c",   0,  0 },
		{ "e",   1,  0 },
		{ "sw", -1,  1 },
		{ "s",   0,  1 },
		{ "se",  1,  1 },
	};

	for(auto const& corner: corners) {
		hash_[name + "_" + corner.suffix] = { idx,
			(x + corner.x) / sizes_[idx].w * 16,
			(y + corner.y) / sizes_[idx].h * 16,
			1.0f / sizes_[idx].w * 16,
			1.0f / sizes_[idx].h * 16,
			16, 16 };
	}
}
