#ifndef LIBS_IMAGE_H
#define LIBS_IMAGE_H

#include "libs/resource.h"
#include "libs/colors.h"
#include "util/rect.h"
#include "util/point.h"
#include "util/defines.h"

class Image : public Resource {
public:
	Image(int w, int h, bool has_alpha) 
		: w(w), h(h), has_alpha_(has_alpha) { }
	virtual ~Image() { }

	virtual void SetPixel(int x, int y, Color c) = 0;
	virtual void Blit(const Image& image) const {
		Blit(image, Rect(0, 0, image.w, image.h));
	}
	virtual void Blit(const Image& image, const Rect& r) const {
		Blit(Rect(0, 0, w, h), image, r);
	}
	virtual void Blit(const Image& image, const Point<int>& p) const {
		Blit(image, Rect(p.x, p.y));
	}
	virtual void Blit(const Rect& rs, const Image& image,
			const Rect& rd) const = 0;
	virtual void Update() = 0;
	virtual void FillBox(Color c) = 0;
	virtual void FillBox(Rect r, Color c) = 0;
	virtual inline void HollowBox(Rect r, Color c, int width=1)
	{
		FillBox(Rect(r.x, r.y, width, r.h), c);
		FillBox(Rect(r.x, r.y, r.w, width), c);
		FillBox(Rect(r.x, r.y+r.h-width, r.w, width), c);
		FillBox(Rect(r.x+r.w-width, r.y, width, r.h), c);
	}

	template <typename T> void DrawLine(Point<T> p1, Point<T> p2, Color c, int line_width=1) {
		int x0(min(max(p1.x, T(0)), T(this->w - line_width))),
		    y0(min(max(p1.y, T(0)), T(this->h - line_width))),
		    x1(min(p2.x, T(this->w - line_width))),
		    y1(min(p2.y, T(this->h - line_width)));
		int dx(abs(x1-x0)), sx(x0<x1 ? 1 : -1);
		int dy(abs(y1-y0)), sy(y0<y1 ? 1 : -1);
		int err((dx>dy ? dx : -dy)/2), e2;

		for(;;) {
			int xx(0);
			int yy(0);
			for(int x(0); x<line_width; x++)
				for(int y(0); y<line_width; y++) {
					SetPixel(x0+x+xx, y0+y+yy, c);
				}
			if(x0 == x1 && y0 == y1) {
				break;
			}
			e2 = err;
			if(e2 > -dx) {
				err -= dy;
				x0 += sx;
			}
			if(e2 < dy) {
				err += dx;
				y0 += sy;
			}
		}
	}

	virtual void RemoveAlphaChannel() = 0;

	virtual inline bool HasAlpha() const { return has_alpha_; }

	const int w, h;

protected:
	bool has_alpha_;

	DISALLOW_COPY_AND_ASSIGN(Image);
};

#endif
