#ifndef WIREFRAMEUI_H
#define WIREFRAMEUI_H

#include <SDL2/SDL_ttf.h>
#include "ui/ui.h"

class WireframeUI : public UI {
public:
	WireframeUI();
	~WireframeUI();

	void Initialize();
	inline bool Active() { return active; }
	void Quit();

	uint32_t Now();
	void Wait(uint32_t tm);

	void GetEvents(vector<Command*>& commands);

	void RedrawScene();

private:
	bool active;
	
	struct SDL_Window* win;
	struct SDL_Renderer* ren;
	TTF_Font* main_font;
};

#endif
