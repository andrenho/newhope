#ifndef UI_IMAGE_H
#define UI_IMAGE_H

struct Color {
	unsigned char r, g, b;
};

class Image {
public:
	virtual ~Image();

	static Image* LoadImage(std::string filename);
	void SetPixelRow(int y, unsigned char* data);
	inline unsigned char const* Data() const { return data; }

	const int W;
	const int H;
	const unsigned char ByteDepth;

protected:
	Image(int w, int h, unsigned char byte_depth);
	unsigned char* data;

private:
	static Image* LoadPNG(std::string filename);
};

#endif
