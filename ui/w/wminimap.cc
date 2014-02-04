#include "ui/w/wminimap.h"

#include <SDL2/SDL.h>

#include "./globals.h"
#include "ui/w/wireframeui.h"

WMinimap::WMinimap(int w, int h, SDL_Renderer& ren)
	: Minimap(w, h), ren(ren), sf(nullptr), texture(nullptr)
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
}


void 
WMinimap::DestroyImage()
{
	if(sf) {
		SDL_FreeSurface(sf);
		sf = nullptr;
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
	pixel += (y * sf->pitch) + (x * sizeof(Uint32));
	*(reinterpret_cast<Uint32*>(pixel)) = (r<<16) | (g<<8) | (b);
}


void 
WMinimap::DrawPoints(std::vector<Point> const& pts, uint8_t r, uint8_t g, uint8_t b)
{
	SDL_Renderer* sr = SDL_CreateSoftwareRenderer(sf);
	SDL_SetRenderDrawColor(sr, r, g, b, SDL_ALPHA_OPAQUE);
	SDL_Point* points = 
		static_cast<SDL_Point*>(calloc(sizeof(SDL_Point), pts.size()));
	int i = 0;
	for(auto const& p : pts) {
		points[i++] = { static_cast<int>(p.X()), static_cast<int>(p.Y()) };
	}
	SDL_RenderDrawLines(sr, points, i);
	SDL_RenderPresent(sr);
	SDL_DestroyRenderer(sr);
}


void 
WMinimap::FinishImage()
{
	texture = SDL_CreateTextureFromSurface(&ren, sf);
}


void 
WMinimap::PresentImage(int x, int y) const
{
	SDL_Rect dest = { x, y, sf->w, sf->h };
	SDL_RenderCopy(&ren, texture, NULL, &dest);
	SDL_RenderPresent(&ren);
}
