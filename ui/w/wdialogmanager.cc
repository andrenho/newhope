#include "ui/w/wdialogmanager.h"

#include <algorithm>
#include <cstdint>

#include "./globals.h"
#include "engine/world.h"
#include "engine/hero.h"
#include "engine/vehicle.h"
#include "engine/resources.h"
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
    
    std::vector<std::string> lines = Wrap(message, static_cast<unsigned int>((win_w-50) / advance));
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
    ui->RedrawScene();
    ui->PresentScene();
}


std::string 
WDialogManager::Question(class Person const& person, std::string message) const
{
    
}


void 
WDialogManager::Shopkeeper(class City& city) const
{
    
    bool closed = false;
    std::map<Resource, SDL_Rect> mrects;
    std::vector<SDL_Rect> crects;
    while(!closed) {
        ShopKeeperDraw(city, mrects, crects);
        closed = ShopKeeperEvents(city, mrects, crects);
    }
}


void 
WDialogManager::ShopKeeperDraw(class City& city, std::map<Resource, SDL_Rect>& mrects, std::vector<SDL_Rect>& crects) const
{
    Hero& hero = world->Hero();

    // draw box
    SDL_Rect r1{ 98, 98, 604, 404 };
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderFillRect(ren, &r1);
    SDL_Rect r2{ 100, 100, 600, 400 };
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderFillRect(ren, &r2);
    WriteTextOnScreen(small_font, "Shopkeeper:", 150, 150, 0, 0, 0);
    WriteTextOnScreen(small_font, "Hero ($ " + std::to_string(hero.Money()) + "):", 
            150, 270, 0, 0, 0);
 
    // draw shopkeeper merchindising
    int x = 150;
    for(auto const& res : ResourceList) {
        mrects[res] = { x, 170, 25, 25 };
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderFillRect(ren, &mrects[res]);
        SDL_Rect r3 = { mrects[res].x+2, 172, 21, 21 };
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderFillRect(ren, &r3);
        WriteTextOnScreen(main_font, std::string(1, static_cast<char>(res)), x+5, 175, 0, 0, 0);
        WriteTextOnScreen(small_font, std::to_string(city.ResourceAmount(res)), x, 200, 0, 0, 0);
        
        x += 35;
    }

    // draw vehicle cargo slots
    x = 150;
    for(unsigned int i=0; i<hero.Vehicle().Model().CargoSlots; ++i) {
        crects.push_back({ x, 290, 25, 25 });
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderFillRect(ren, &crects[i]);
        SDL_Rect r3 = { crects[i].x+2, 292, 21, 21 };
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderFillRect(ren, &r3);
        CargoSlot const& cs = hero.Vehicle().Cargo(i);
        if(cs.Cargo != Resource::NOTHING) {
            WriteTextOnScreen(main_font, std::string(1, static_cast<char>(cs.Cargo)), x+5, 295, 0, 0, 0);
            WriteTextOnScreen(small_font, std::to_string(cs.Amount), x, 320, 0, 0, 0);
        }
        
        x += 35;
    }

	SDL_RenderPresent(ren);
}


bool
WDialogManager::ShopKeeperEvents(class City& city, std::map<Resource, SDL_Rect> const& mrects, std::vector<SDL_Rect> const& crects) const
{
    Resource dragging = Resource::NOTHING;
    int cargo_slot = -1;
    enum { BUYING, SELLING, NOTHING } action = NOTHING;

    auto in_rect = [](int32_t& x, int32_t& y, SDL_Rect const& rect) -> bool {
        return (x >= rect.x && x < rect.x+rect.w && y >= rect.y && y < rect.y+rect.h);
    };

    for(;;) {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            switch(e.type) {

            case SDL_KEYDOWN:
                if(e.key.keysym.sym == SDLK_ESCAPE) {
                    return true;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                if(e.button.button == SDL_BUTTON_LEFT) {
                    // check if dragging from shopkeeper
                    for(auto const& mrect: mrects) {
                        if(in_rect(e.button.x, e.button.y, mrect.second)) {
                            dragging = mrect.first;
                            action = BUYING;
                        }
                    }
                    // check if dragging from cargo
                    int slot = 0;
                    for(auto const& crect: crects) {
                        if(in_rect(e.button.x, e.button.y, crect)) {
                            cargo_slot = slot;
                            action = SELLING;
                        }
                        ++slot;
                    }
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if(e.button.button == SDL_BUTTON_LEFT) {
                    // check if dropping on vehicle cargo (buying)
                    if(action == BUYING) {
                        int slot = 0;
                        for(auto const& crect: crects) {
                            if(in_rect(e.button.x, e.button.y, crect)) {
                                // TODO - shift
                                world->Hero().Buy(city, dragging, std::min(100U, city.ResourceAmount(dragging)));
                            }
                            ++slot;
                        }

                    // check if dropping on shopkeeper (selling)
                    } else if(action == SELLING) {
                        for(auto const& mrect: mrects) {
                            if(in_rect(e.button.x, e.button.y, mrect.second)) {
                                // TODO - shift
                                world->Hero().Sell(city, cargo_slot, std::min(100U, world->Hero().Vehicle().Cargo(cargo_slot).Amount));
                            }
                        }
                    }

                    action = NOTHING;
                }
                break;

            default:
                ;
            }
        }
    }
    return false;
}


int
WDialogManager::WriteTextOnScreen(TTF_Font* font, std::string text, 
        int x, int y, uint8_t r, uint8_t g, uint8_t b) const
{
    if(text != "") {
		SDL_Surface* sf = TTF_RenderUTF8_Solid(font, text.c_str(), 
				SDL_Color{r,g,b,0});
		SDL_Texture* txt = SDL_CreateTextureFromSurface(ren, sf);
        SDL_Rect rect = { x, y, sf->w, sf->h };
		SDL_RenderCopy(ren, txt, NULL, &rect);
		SDL_FreeSurface(sf);
		SDL_DestroyTexture(txt);
    }
	return TTF_FontLineSkip(main_font);
}


// vim: ts=4:sw=4:sts=4:expandtab
