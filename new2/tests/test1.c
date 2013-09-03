#include <stdio.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_timer.h"
#include "SDL2/SDL_image.h"

int main()
{
	SDL_Window* win;
	SDL_Renderer* ren;
	int x, y;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(800, 600, 0, &win, &ren);
	SDL_Surface* sf = IMG_Load("temp.bmp");
	SDL_Texture* tx = SDL_CreateTextureFromSurface(ren, sf);

	SDL_RendererInfo info;
	SDL_GetRendererInfo(ren, &info);
	printf("SDL_RENDERER_ACCELERATED = %d\n", info.flags & SDL_RENDERER_ACCELERATED);
	printf("SDL_RENDERER_SOFTWARE = %d\n", info.flags & SDL_RENDERER_SOFTWARE);
	printf("SDL_RENDERER_PRESENTVSYNC = %d\n", info.flags & SDL_RENDERER_PRESENTVSYNC);
	
	for(;;) {
		Uint32 t = SDL_GetTicks();
		for(x=0; x<800; x+=16) {
			for(y=0; y<600; y+=16) {
				SDL_Rect src = { 0, 0, 16, 16 };
				SDL_Rect dst = { x, y, 16, 16 };
				SDL_RenderCopy(ren, tx, &src, &dst);
			}
		}
		Uint32 t1 = SDL_GetTicks();
		SDL_RenderPresent(ren);
		Uint32 t2 = SDL_GetTicks();
		printf("a. %ld\n", t1 - t);
		printf("b. %ld\n", t2 - t);
	}
}
