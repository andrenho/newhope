#ifndef UI_W_WMINIMAP_H_
#define UI_W_WMINIMAP_H_

#include "ui/minimap.h"

class WMinimap : public Minimap {
public:
	WMinimap(int w, int h, struct SDL_Renderer& ren);
	virtual ~WMinimap();

	void DestroyImage();

protected:
	void CreateImage(int w, int h);
	void DrawPoint(int x, int y, uint8_t r, uint8_t g, uint8_t b);
	void FinishImage();
	void PresentImage(int x, int y) const;

private:
	struct SDL_Renderer& ren;
	struct SDL_Surface* sf;
	struct SDL_Texture* texture;
};

#endif  // UI_W_WMINIMAP_H_
