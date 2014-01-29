// Copyright 2014 <Imperial Software>

#include "ui/w/wireframeui.h"

#include <SDL2/SDL.h>
#include <cstdlib>

#include "./globals.h"
#include "engine/command.h"
#include "engine/hero.h"
#include "engine/point.h"
#include "engine/rectangle.h"
#include "engine/world.h"

WireframeUI::WireframeUI()
	: active(true), win(nullptr), ren(nullptr), main_font(nullptr),
	  rx(0), ry(0)
{
	// initialize SDL
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "\nUnable to initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	// initialize window
	if(!(win = SDL_CreateWindow("NewHope", SDL_WINDOWPOS_CENTERED, 
			SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE))) {
		fprintf(stderr, "\nUnable to intialize window: %s\n", SDL_GetError());
		exit(1);
	}

	// initialize renderer
	if(!(ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE))) {
		fprintf(stderr, "\nUnable to intialize renderer: %s\n", SDL_GetError());
		exit(1);
	}

	// initialize TTF_Init
	if(TTF_Init() == -1) {
		fprintf(stderr, "\nError initializing SDL2_ttf: %s\n", TTF_GetError());
		exit(1);
	}

	// load font
	main_font = TTF_OpenFont(DATADIR "/PressStart2P.ttf", 16);
	if(!main_font) {
		fprintf(stderr, "\nUnable to load font: %s\n", TTF_GetError());
		exit(1);
	}

}


WireframeUI::~WireframeUI()
{
	TTF_CloseFont(main_font);
	TTF_Quit();
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
}


void 
WireframeUI::Initialize()
{
}


void
WireframeUI::Quit()
{
	active = false;
}


uint32_t 
WireframeUI::Now() const
{
	return 0;
}


void 
WireframeUI::Wait(uint32_t tm) const
{
}


void 
WireframeUI::GetEvents(std::vector<Command*>& cmds) const
{
	SDL_Event e;
	while(SDL_PollEvent(&e)) {
		switch(e.type) {
		case SDL_QUIT:
			cmds.push_back(new QuitCommand());
			break;
		}
	}
}


void
WireframeUI::RedrawScene() const
{
	std::vector<Object*> objects;
	std::vector<uint8_t[10]> tiles;

	Point hero_pos = world->Hero().Position();
	CenterScreen(hero_pos);

	Rectangle const* visible_area = GetVisibleArea();
	GetVisibleTiles(tiles, *visible_area);
	GetVisibleObjects(objects, *visible_area);
	delete visible_area;

	RenderScene(tiles, objects);
}


void 
WireframeUI::CenterScreen(Point const& p) const
{
	int win_w, win_h;
	SDL_GetWindowSize(win, &win_w, &win_h);

	rx = -p.X() * Z + (win_w/2);
	ry = -p.Y() * Z + (win_h/2);
}


Rectangle const*
WireframeUI::GetVisibleArea() const
{
	int win_w, win_h;
	SDL_GetWindowSize(win, &win_w, &win_h);

	Point p1((-rx) / Z - 5, (-ry) / Z - 5);
	Point p2((-rx + (win_w)) / Z + 5, (-ry + (win_h)) / Z + 5);
	return new Rectangle(p1, p2);
}


void 
WireframeUI::GetVisibleTiles(std::vector<uint8_t[10]>& tiles,
		Rectangle const& area) const
{
}


void 
WireframeUI::GetVisibleObjects(std::vector<Object*>& objects,
		Rectangle const& area) const
{
}


void 
WireframeUI::RenderScene(std::vector<uint8_t[10]> const& tiles,
		std::vector<Object*> const& objects) const
{
}
