#ifndef UI_W_WIREFRAMEUI_H_
#define UI_W_WIREFRAMEUI_H_

#include <SDL2/SDL_ttf.h>
#include <vector>

#include "engine/block.h"
#include "ui/ui.h"

class Object;
class Point;
class Rectangle;

class WireframeUI final : public UI {
public:
	WireframeUI();
	~WireframeUI();

	void Initialize();
	inline bool Active() const { return active; }
	void Quit();

	uint32_t Now() const;
	void Wait(uint32_t tm) const;

	void GetEvents(std::vector<Command*>& commands) const;

	Rectangle& GetVisibleArea(Rectangle& r) const;
	void RedrawScene() const;
	void PresentScene() const;

	void ShowMinimap() const;

private:
	WireframeUI(const WireframeUI&);
	WireframeUI& operator=(const WireframeUI&);

	void CenterScreen(Point const& p) const;
	void GetVisibleTiles(std::vector<Block[10]>& tiles,
			Rectangle const& area) const;
	void RenderScene(Rectangle const& rect) const;
	void DrawTile(int x, int y) const;
	void DrawObject(Object const& object) const;
	void DrawShape(struct cpBody* body, struct cpShape* shape) const;
	static void DrawStaticShape(struct cpBody *body, struct cpShape *shape, 
			void* data);
	void RenderCircle(double x1, double y1, double r) const;

	bool active;
	
	struct SDL_Window* win;
	struct SDL_Renderer* ren;
	TTF_Font* main_font;

	mutable double rx, ry;

	const double Z = 10;
};

#endif  // UI_W_WIREFRAMEUI_H_
