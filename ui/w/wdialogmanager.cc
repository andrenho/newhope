#include "ui/w/wdialogmanager.h"

#include <SDL2/SDL.h>

#include <vector>

#include "./globals.h"
#include "ui/ui.h"

WDialogManager::WDialogManager(struct SDL_Window* win, struct SDL_Renderer* ren)
    : win(win), ren(ren), main_font(nullptr)
{
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
		SDL_Surface* sf = TTF_RenderUTF8_Solid(main_font, line.c_str(), 
				SDL_Color{255,255,255,0});
		SDL_Texture* txt = SDL_CreateTextureFromSurface(ren, sf);
        SDL_Rect r = { 25, y, sf->w, sf->h };
		SDL_RenderCopy(ren, txt, NULL, &r);
		SDL_FreeSurface(sf);
		SDL_DestroyTexture(txt);
		y += TTF_FontLineSkip(main_font);
	}
	SDL_RenderPresent(ren);
	
    ui->WaitForKeypress();
}


void 
WDialogManager::Shopkeeper(class City& city) const
{
    (void) city;
    // TODO
}


// vim: ts=4:sw=4:sts=4:expandtab
