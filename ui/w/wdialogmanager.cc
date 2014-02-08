#include "ui/w/wdialogmanager.h"

#include <SDL2/SDL.h>

#include <vector>

#include "./globals.h"
#include "ui/ui.h"

WDialogManager::WDialogManager(struct SDL_Window* win, struct SDL_Renderer* ren)
    : win(win), ren(ren), small_font(nullptr), main_font(nullptr)
{
    // initialize TTF_Init
    if(TTF_Init() == -1) {
        fprintf(stderr, "\nError initializing SDL2_ttf: %s\n", TTF_GetError());
        exit(1);
    }

    // load font
    main_font = TTF_OpenFont(DATADIR "/PressStart2P.ttf", 16);
    small_font = TTF_OpenFont(DATADIR "/PressStart2P.ttf", 8);
    if(!main_font || !small_font) {
        fprintf(stderr, "\nUnable to load font: %s\n", TTF_GetError());
        exit(1);
    }
}


WDialogManager::~WDialogManager()
{
    TTF_CloseFont(main_font);
    TTF_Quit();
}

void 
WDialogManager::Speech(class Person const& person, std::string message) const
{
    (void) person;

    // get screen size
    int win_w, win_h;
    SDL_GetWindowSize(win, &win_w, &win_h);

    // find font size
    int advance;
    TTF_GlyphMetrics(main_font, 'A', NULL, NULL, NULL, NULL, &advance);
    
    std::vector<std::string> lines = Wrap(message, (win_w-50) / advance);
    int nlines = static_cast<int>(lines.size());
    
    // draw black box
    SDL_Rect r = { 
        0, win_h - (nlines * TTF_FontLineSkip(main_font)) - 50, 
        win_w, (nlines * TTF_FontLineSkip(main_font)) + 50 
    };
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderFillRect(ren, &r);
    int y = win_h - (nlines * TTF_FontLineSkip(main_font)) - 25;
    
    // write text on screen
    for(auto const& line: lines) {
        y += WriteTextOnScreen(main_font, line, 25, y, 255, 255, 255);
	}
	SDL_RenderPresent(ren);
	
    ui->WaitForKeypress();
}


void 
WDialogManager::Shopkeeper(class City& city) const
{
    // draw box
    SDL_Rect r1{ 98, 98, 604, 404 };
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderFillRect(ren, &r1);
    SDL_Rect r2{ 100, 100, 600, 400 };
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderFillRect(ren, &r2);
    WriteTextOnScreen(small_font, "Shopkeeper:", 150, 150, 0, 0, 0);
    WriteTextOnScreen(small_font, "Hero:", 150, 270, 0, 0, 0);
    
    (void) city;
    // TODO

	SDL_RenderPresent(ren);
    ui->WaitForKeypress();
}


int
WDialogManager::WriteTextOnScreen(TTF_Font* font, std::string text, 
        int x, int y, uint8_t r, uint8_t g, uint8_t b) const
{
		SDL_Surface* sf = TTF_RenderUTF8_Solid(font, text.c_str(), 
				SDL_Color{r,g,b,0});
		SDL_Texture* txt = SDL_CreateTextureFromSurface(ren, sf);
        SDL_Rect rect = { x, y, sf->w, sf->h };
		SDL_RenderCopy(ren, txt, NULL, &rect);
		SDL_FreeSurface(sf);
		SDL_DestroyTexture(txt);
		return TTF_FontLineSkip(main_font);
}


// vim: ts=4:sw=4:sts=4:expandtab
