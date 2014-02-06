#include "ui/w/wminimap.h"

#include <SDL2/SDL.h>

#include <iostream>

#include "./globals.h"
#include "ui/w/wireframeui.h"

WMinimap::WMinimap(int w, int h, SDL_Renderer& ren)
	: Minimap(w, h), ren(ren), sr(nullptr), sf(nullptr), texture(nullptr)
{
}


WMinimap::~WMinimap()
{
}


/********************************************************************************/

void 
WMinimap::CreateImage(int w, int h)
{
	sf = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
	sr = SDL_CreateSoftwareRenderer(sf);
}


void 
WMinimap::DestroyImage()
{
	if(sf) {
		SDL_FreeSurface(sf);
		sf = nullptr;
		SDL_DestroyRenderer(sr);
	}

	if(texture) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
}


void 
WMinimap::DrawPoint(int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
	Uint8* pixel = reinterpret_cast<Uint8*>(sf->pixels);
	pixel += (y * static_cast<int>(sf->pitch)) + (x * static_cast<int>(sizeof(Uint32)));
	*(reinterpret_cast<Uint32*>(pixel)) = static_cast<Uint32>((r<<16) | (g<<8) | (b));
}


void 
WMinimap::DrawPoints(std::vector<Point> const& pts, uint8_t r, uint8_t g, uint8_t b)
{
	SDL_SetRenderDrawColor(sr, r, g, b, SDL_ALPHA_OPAQUE);
	SDL_Point* points = 
		static_cast<SDL_Point*>(calloc(sizeof(SDL_Point), pts.size()));
	int i = 0;
	for(auto const& p : pts) {
		points[i++] = { static_cast<int>(p.X()), static_cast<int>(p.Y()) };
	}
	SDL_RenderDrawLines(sr, points, i);
}


void 
WMinimap::DrawRectangleScreen(int x1, int y1, int x2, int y2, bool fill, 
		uint8_t r, uint8_t g, uint8_t b) const
{
	SDL_SetRenderDrawColor(&ren, r, g, b, SDL_ALPHA_OPAQUE);
	SDL_Rect rect = { x1, y1, x2, y2 };
	if(fill) {
		SDL_RenderFillRect(&ren, &rect);
	} else {
		SDL_RenderDrawRect(&ren, &rect);
	}
}


void 
WMinimap::DrawRectangle(int x1, int y1, int x2, int y2, bool fill, 
		uint8_t r, uint8_t g, uint8_t b) const
{
	SDL_SetRenderDrawColor(sr, r, g, b, SDL_ALPHA_OPAQUE);
	SDL_Rect rect = { x1, y1, x2, y2 };
	if(fill) {
		SDL_RenderFillRect(sr, &rect);
	} else {
		SDL_RenderDrawRect(sr, &rect);
	}
}


void 
WMinimap::FinishImage()
{
	SDL_RenderPresent(sr);
	texture = SDL_CreateTextureFromSurface(&ren, sf);
}


void 
WMinimap::PresentImage(int x, int y) const
{
	SDL_Rect dest = { x, y, sf->w, sf->h };
	SDL_RenderCopy(&ren, texture, NULL, &dest);
	SDL_RenderPresent(&ren);
}


void
WMinimap::UpdateScreen() const
{
	SDL_RenderPresent(&ren);
}
