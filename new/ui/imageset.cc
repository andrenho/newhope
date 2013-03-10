#include "defines.h"

#include <GL/gl.h>

Imageset::Imageset()
	: c_texture(0)
{
	static vector<string> images = { "tiles.png" };
	n_textures = images.size();
	texture_ = new unsigned int[n_textures];
	
	for(string image: images) {
		LoadImage(image);
	}

	// texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}


Imageset::~Imageset()
{
	delete[] texture_;
}


void
Imageset::LoadImage(string filename)
{
	// load tiles
	Image* tileset = Image::LoadImage(filename);
	ImageRGB t = *((ImageIndexed*)tileset);
	glGenTextures(1, &texture_[c_texture]);
	glBindTexture(GL_TEXTURE_2D, texture_[c_texture]);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, t.W, t.H, 0, GL_RGBA, 
			GL_UNSIGNED_BYTE, t.Data());
	delete tileset;

	hash_["grass_c"] = { 0, 1.0f/(float)t.W*16, 2.0f/(float)t.H*16, 
		1.0f/(float)t.W*16, 1.0f/(float)t.H*16 };
	hash_["water_c"] = { 0, 1.0f/(float)t.W*16, 6.0f/(float)t.H*16, 
		1.0f/(float)t.W*16, 1.0f/(float)t.H*16 };

	c_texture++;
}
