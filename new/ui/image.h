#ifndef UI_IMAGE_H
#define UI_IMAGE_H

class Image {
public:
	static Image* LoadImage(std::string filename);

	const int W;
	const int H;

protected:
	Image(int w, int h) : W(w), H(h) { }

private:
	static Image* LoadPNG(std::string filename);
};

#endif
