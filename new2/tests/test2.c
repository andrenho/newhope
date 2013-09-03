#include "SDL2/SDL.h"

int main()
{
	SDL_Window* win;
	SDL_Renderer *ren;

	// initialize SDL
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(800, 600, 0, &win, &ren);

	// create backbuffer and renderer
	SDL_Texture* tx = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_TARGET, 800, 600);
	SDL_SetRenderTarget(ren, tx);
	SDL_SetRenderDrawColor(ren, 0, 255, 255, 255);
	SDL_RenderClear(ren);

	// paste backbuffer on the screen
	SDL_SetRenderTarget(ren, NULL);
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	SDL_RenderClear(ren);
	SDL_Rect r = { 0, 0, 800, 600 };
	SDL_RenderCopy(ren, tx, &r, &r);
	SDL_RenderPresent(ren);

	SDL_Delay(2000);
}
