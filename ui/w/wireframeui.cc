#include "wireframeui.h"

#include <cstdlib>

#include <SDL2/SDL.h>

WireframeUI::WireframeUI()
	: active(true)
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
	/*
	if(TTF_Init() == -1) {
		fprintf(stderr, "\nError initializing SDL2_ttf: %s\n", TTF_GetError());
		exit(1);
	}

	// load font
	ui.main_font = TTF_OpenFont(DATADIR "/PressStart2P.ttf", 16);
	if(!ui.main_font) {
		fprintf(stderr, "\nUnable to load font: %s\n", TTF_GetError());
		exit(1);
	}*/

}


WireframeUI::~WireframeUI()
{
	// TODO - destroy stuff!!
}


void 
WireframeUI::Initialize()
{
}


uint32_t 
WireframeUI::Now()
{
	return 0;
}


void 
WireframeUI::Wait(uint32_t tm)
{
}
