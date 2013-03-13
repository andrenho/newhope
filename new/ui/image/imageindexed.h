#ifndef UI_IMAGEINDEXED_H
#define UI_IMAGEINDEXED_H

#include "defines.h"

class ImageIndexed : public Image {
public:
	ImageIndexed(int w, int h, unsigned char bytedepth) : 
		Image(w, h, bytedepth), transparent_(-1) { }

	void SetPaletteColor(unsigned char index, Color c) {
		palette_[index] = c;
	}

	void SetTransparentIndex(unsigned char index) {
		transparent_ = index;
		palette_[index].a = 255;
	}

	Color GetPixel(int x, int y) const {
		return palette_[data[(x*ByteDepth)+(y*W*ByteDepth)]];
	}

	inline unsigned char TransparentIndex() { return transparent_; }

private:
	Color palette_[256];
	unsigned char transparent_;
};

#endif
