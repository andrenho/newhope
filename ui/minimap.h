#ifndef UI_MINIMAP_H_
#define UI_MINIMAP_H_

#include <cstdint>
#include <cstdio>
#include <vector>

#include "engine/point.h"

class Minimap {
public:
	Minimap(int w, int h);
	virtual ~Minimap();

	void Initialize();
	void Draw(int x, int y) const;
	virtual void DestroyImage() = 0;

	const int W, H;

protected:

	virtual void CreateImage(int w, int h) = 0;
	virtual void DrawPoint(int x, int y, uint8_t r, uint8_t g, uint8_t b) = 0;
	virtual void DrawPoints(std::vector<Point> const& pts, 
			uint8_t r, uint8_t g, uint8_t b) = 0;
	virtual void FinishImage() = 0;
	
	virtual void PresentImage(int x, int y) const = 0;
};

#endif  // UI_MINIMAP_H_