#include "defines.h"

using namespace std;

ImageRGB::ImageRGB(ImageIndexed const& idx)
	: Image(idx.W, idx.H, 4)
{
	for(int x=0; x<W; x++) {
		for(int y=0; y<H; y++) {
			SetPixel(x, y, idx.GetPixel(x, y));
		}
	}
}


void 
ImageRGB::SetPixel(int x, int y, Color c) {
	int p = (x * ByteDepth) + (y * W * ByteDepth);
	data[p] = c.r;
	data[p+1] = c.g;
	data[p+2] = c.b;
	data[p+3] = 0;
}
