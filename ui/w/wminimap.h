#ifndef UI_W_WMINIMAP_H_
#define UI_W_WMINIMAP_H_

#include <vector>
using namespace std;
#include "ui/minimap.h"

class WMinimap final : public Minimap {
public:
    WMinimap(int w, int h, struct SDL_Renderer& ren);
    ~WMinimap();

    void DestroyImage();

protected:
    void CreateImage(int w, int h) override;
    void DrawPoint(int x, int y, uint8_t r, uint8_t g, uint8_t b) override;
    void DrawPoints(vector<Point> const& pts, uint8_t r, uint8_t g, uint8_t b) override;
    void DrawRectangle(int x1, int y1, int x2, int y2, bool fill, uint8_t r, uint8_t g, uint8_t b) const override;
    void DrawRectangleScreen(int x1, int y1, int x2, int y2, bool fill, uint8_t r, uint8_t g, uint8_t b) const override;
    void DrawWaitingScreen() const override {}
    void FinishImage() override;
    void PresentImage(int x, int y) const override;
    void UpdateScreen() const override;

private:
    WMinimap(const WMinimap&);
    WMinimap& operator=(const WMinimap&);

    struct SDL_Renderer& ren;
    struct SDL_Renderer* sr;
    struct SDL_Surface* sf;
    struct SDL_Texture* texture;
};

#endif  // UI_W_WMINIMAP_H_


// vim: ts=4:sw=4:sts=4:expandtab
