#ifndef UI_W_WIREFRAMEUI_H_
#define UI_W_WIREFRAMEUI_H_

#include <memory>
#include <vector>
using namespace std;

#include "engine/block.h"
#include "ui/ui.h"

class Object;
class Point;
class Rectangle_;

class WireframeUI final : public UI {
public:
    WireframeUI();
    ~WireframeUI();

    WireframeUI(const WireframeUI&) = delete;
    WireframeUI& operator=(const WireframeUI&) = delete;

    void Initialize() override;
    inline bool Active() const override { return active; }
    void Quit() override;

    uint32_t Now() const override;
    void Wait(uint32_t tm) const override;

    vector<unique_ptr<Command>> GetEvents() const override;

    Rectangle_& GetVisibleArea(Rectangle_& r) const override;
    void RedrawScene() const override;
    void PresentScene() const override;

    void ShowMinimap() const override;

    void WaitForKeypress() const override;

private:
    void CenterScreen(Point const& p) const;
    void GetVisibleTiles(vector<Block[10]>& tiles,
            Rectangle_ const& area) const;
    void RenderScene(Rectangle_ const& rect) const;
    void DrawTile(int x, int y) const;
    void DrawObject(Object const& object) const;
    void DrawShape(struct cpBody* body, struct cpShape* shape) const;
    static void DrawStaticShape(struct cpBody *body, struct cpShape *shape, 
            void* data);
    void RenderCircle(double x1, double y1, double r) const;
    
    bool active = true;
    struct SDL_Window* win = nullptr;
    struct SDL_Renderer* ren = nullptr;
    mutable double rx = 0, ry = 0;
    const double Z = 10;
};

#endif  // UI_W_WIREFRAMEUI_H_


// vim: ts=4:sw=4:sts=4:expandtab
