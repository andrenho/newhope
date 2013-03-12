#ifndef UI_IMAGESET_H
#define UI_IMAGESET_H

#include <GL/gl.h>

typedef struct Reference { 
	int idx;
	float x, y, w, h;
} Reference;


class Imageset {
public:
	Imageset();
	~Imageset();

	const Reference operator[](std::string s) const { return hash_.at(s); }
	unsigned int const* Texture() const { return texture_; }

private:
	typedef struct ImageSize { float w, h; } ImageSize;

	void LoadImage(string filename);
	void SetupImageMap();
	void SetupImageTerrain(string name, int idx, float x, float y);
	void SetupFont(string name, int idx, int w, int h);
	void SetupDialog(string name, int idx);

	map<string, Reference> hash_;
	map<int, ImageSize> sizes_;
	GLuint* texture_;
	int n_textures, c_texture;
};

#endif
