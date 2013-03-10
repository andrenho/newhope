#ifndef UI_IMAGESET_H
#define UI_IMAGESET_H

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
	void LoadImage(string filename);

	std::map<std::string, Reference> hash_;
	unsigned int* texture_;
	int n_textures, c_texture;
};

#endif
