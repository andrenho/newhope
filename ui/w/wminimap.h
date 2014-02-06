#ifndef UI_W_WMINIMAP_H_
#define UI_W_WMINIMAP_H_

#include <vector>
#include "ui/minimap.h"

class WMinimap final : public Minimap {
public:
	WMinimap(int w, int h, struct SDL_Renderer& ren);
	~WMinimap();

	void DestroyImage();

protected:
	void CreateImage(int w, int h);
	void DrawPoint(int x, int y, uint8_t r, uint8_t g, uint8_t b);
	void DrawPoints(std::vector<Point> const& pts, 
			uint8_t r, uint8_t g, uint8_t b);
	void DrawRectangle(int x1, int y1, int x2, int y2, 
			bool fill, uint8_t r, uint8_t g, uint8_t b) const;
	void FinishImage();
	void PresentImage(int x, int y) const;
	void UpdateScreen() const;

private:
	WMinimap(const WMinimap&);
	WMinimap& operator=(const WMinimap&);

	struct SDL_Renderer& ren;
	struct SDL_Surface* sf;
	struct SDL_Texture* texture;
};

#endif  // UI_W_WMINIMAP_H_
