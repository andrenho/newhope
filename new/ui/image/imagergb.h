#ifndef UI_IMAGERGB_H
#define UI_IMAGERGB_H

#include "defines.h"

class ImageRGB : public Image {
public:
	ImageRGB(ImageIndexed const& idx);
	void SetPixel(int x, int y, Color c);
};

#endif
