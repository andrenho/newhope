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
	virtual void DrawLine(Point<int> p1, Point<int> p2, Color c, int w=1) = 0;
	virtual void RemoveAlphaChannel() = 0;

	virtual inline bool HasAlpha() const { return has_alpha_; }

	const int w, h;

protected:
	bool has_alpha_;

	DISALLOW_COPY_AND_ASSIGN(Image);
};

#endif
