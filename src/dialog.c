#include "dialog.h"

#include <lauxlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

int dialog_message(lua_State* L)
{
	// get screen size
	int win_w, win_h;
	SDL_GetWindowSize(ui.win, &win_w, &win_h);

	// check parameters
	luaL_checktype(L, 1, LUA_TTABLE);
	char* text = strdup(luaL_checkstring(L, 2));

	// display message
	int advance;
	TTF_GlyphMetrics(ui.main_font, 'A', NULL, NULL, NULL, NULL, &advance);
	int lines = ui_wrap_text(text, (win_w-50) / advance);
	SDL_Rect r = { 0, win_h - (lines * TTF_FontLineSkip(ui.main_font)) - 50, 
		win_w, (lines * TTF_FontLineSkip(ui.main_font)) + 50 };
	SDL_SetRenderDrawColor(ui.ren, 0, 0, 0, 255);
	SDL_RenderFillRect(ui.ren, &r);
	int y = win_h - (lines * TTF_FontLineSkip(ui.main_font)) - 25;
	char *token;
#ifdef __MINGW32__
	while((token = strtok(text, "\n")) != NULL) {
#else
	char* saveptr = NULL;
	while((token = strtok_r(text, "\n", &saveptr)) != NULL) {
#endif
		SDL_Surface* sf = TTF_RenderUTF8_Solid(ui.main_font, token, 
				(SDL_Color){255,255,255});
		SDL_Texture* txt = SDL_CreateTextureFromSurface(ui.ren, sf);
		SDL_RenderCopy(ui.ren, txt, NULL, &(SDL_Rect){ 25, y, sf->w, sf->h });
		SDL_FreeSurface(sf);
		SDL_DestroyTexture(txt);
		y += TTF_FontLineSkip(ui.main_font);
		text = NULL;
	}
	SDL_RenderPresent(ui.ren);
	
	// wait for keypress
	for(;;) {
		SDL_Event e;
		SDL_PollEvent(&e);
		const Uint8* k = SDL_GetKeyboardState(NULL);
		if(k[SDL_SCANCODE_SPACE])
			break;
		SDL_Delay(1000.0/60.0);
	}
	for(;;) {
		SDL_Event e;
		SDL_PollEvent(&e);
		const Uint8* k = SDL_GetKeyboardState(NULL);
		if(!k[SDL_SCANCODE_SPACE])
			break;
		SDL_Delay(1000.0/60.0);
	}

	free(text);

	return 0;
}
