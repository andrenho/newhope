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
	Point hero_pos = world->Hero().Position();
	CenterScreen(hero_pos);

	Rectangle const* visible_area = GetVisibleArea();
	RenderScene(*visible_area);
	delete visible_area;
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
WireframeUI::RenderScene(Rectangle const& rect) const
{
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	SDL_RenderClear(ren);

	// draw tiles
	for(int x=rect.P1().X(); x<rect.P2().X(); x++) {
		for(int y=rect.P1().Y(); y<rect.P2().Y(); y++) {
			DrawTile(x, y);
		}
	}

	// draw objects
	for(auto const& object: world->Objects()) {
		DrawObject(*object);
	}
	
	SDL_RenderPresent(ren);
}


void 
WireframeUI::DrawTile(int x, int y) const
{
	Block b[10];
	int n = world->Tiles(b, x, y);
	if(n > 0) {
		Block block = b[n-1];
		switch(block) {
		case Block::GRASS:
			SDL_SetRenderDrawColor(ren, 230, 255, 230, 255); break;
		default:
			SDL_SetRenderDrawColor(ren, 255, 255, 255, 255); break;
		}

		SDL_Rect rect = { 
			static_cast<int>(x*Z+rx), 
			static_cast<int>(y*Z+ry), 
			static_cast<int>(Z), 
			static_cast<int>(Z) };
		SDL_RenderFillRect(ren, &rect);
	}
}


void 
WireframeUI::DrawObject(Object const& object) const
{
	const Person* person = nullptr;

	if((person = dynamic_cast<Person const*>(&object)) != nullptr) {
		Point pos = object.Position();
		SDL_SetRenderDrawColor(ren, 0, 128, 0, 255);
		RenderCircle(pos.X()*Z+rx, pos.Y()*Z+ry, person->Radius()*Z);
	}
}


void
WireframeUI::RenderCircle(double x1, double y1, double r) const
{
	double x=r, y=0;
	int radiusError = 1-x;
	int i = 0;
	static SDL_Point circle_pixel[1000];

	while(x >= y) {
		circle_pixel[i++] = SDL_Point{static_cast<int>( x + x1), static_cast<int>( y + y1) };
		circle_pixel[i++] = SDL_Point{static_cast<int>( y + x1), static_cast<int>( x + y1) };
		circle_pixel[i++] = SDL_Point{static_cast<int>(-x + x1), static_cast<int>( y + y1) };
		circle_pixel[i++] = SDL_Point{static_cast<int>(-y + x1), static_cast<int>( x + y1) };
		circle_pixel[i++] = SDL_Point{static_cast<int>(-x + x1), static_cast<int>(-y + y1) };
		circle_pixel[i++] = SDL_Point{static_cast<int>(-y + x1), static_cast<int>(-x + y1) };
		circle_pixel[i++] = SDL_Point{static_cast<int>( x + x1), static_cast<int>(-y + y1) };
		circle_pixel[i++] = SDL_Point{static_cast<int>( y + x1), static_cast<int>(-x + y1) };
		y++;
		if(radiusError<0) {
			radiusError += 2*y+1;
		} else {
			x--;
			radiusError += 2*(y-x+1);
		}
	}

	SDL_RenderDrawPoints(ren, circle_pixel, i);
}


