#include "defines.h"

#include <GL/gl.h>

Imageset::Imageset()
{
	// load tiles
	Image* tileset = Image::LoadImage("tiles.png");
	ImageRGB t = *((ImageIndexed*)tileset);
	glGenTextures(1, &texture_[0]);
	glBindTexture(GL_TEXTURE_2D, texture_[0]);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, t.W, t.H, 0, GL_RGBA, GL_UNSIGNED_BYTE,
			t.Data());
	delete tileset;
	hash_["grass_c"] = { 0, 1.0f/(float)t.W*16, 2.0f/(float)t.H*16, 
		1.0f/(float)t.W*16, 1.0f/(float)t.H*16 };

	// texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}
