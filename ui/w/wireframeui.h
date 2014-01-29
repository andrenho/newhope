// Copyright 2014 <Imperial Software>

#ifndef UI_W_WIREFRAMEUI_H_
#define UI_W_WIREFRAMEUI_H_

#include <SDL2/SDL_ttf.h>
#include <vector>

#include "ui/ui.h"

class Object;
class Point;
class Rectangle;

class WireframeUI : public UI {
public:
	WireframeUI();
	~WireframeUI();

	void Initialize();
	inline bool Active() const { return active; }
	void Quit();

	uint32_t Now() const;
	void Wait(uint32_t tm) const;

	void GetEvents(std::vector<Command*>& commands) const;

	void RedrawScene() const;

private:
	void CenterScreen(Point const& p) const;
	Rectangle const* GetVisibleArea() const;
	void GetVisibleTiles(std::vector<uint8_t[10]>& tiles,
			Rectangle const& area) const;
	void GetVisibleObjects(std::vector<Object*>& objects,
			Rectangle const& area) const;
	void RenderScene(std::vector<uint8_t[10]> const& tiles,
			std::vector<Object*> const& objects) const;

	bool active;
	
	struct SDL_Window* win;
	struct SDL_Renderer* ren;
	TTF_Font* main_font;

	mutable double rx, ry;

	const double Z = 10;
};

#endif  // UI_W_WIREFRAMEUI_H_
