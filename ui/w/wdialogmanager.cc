#include "ui/w/wdialogmanager.h"

#include <algorithm>
#include <climits>
#include <cstdint>
#include <iostream>
#include <string>
using namespace std;

#include "./main.h"
#include "engine/world.h"
#include "engine/hero.h"
#include "engine/vehicle.h"
#include "engine/resources.h"
#include "engine/npc/bartender.h"
#include "engine/npc/banker.h"
#include "engine/npc/shopkeeper.h"
#include "ui/ui.h"
#include "util/stdio.h"

#ifdef __MINGW32__
#include <sstream>
namespace std
{
    inline string to_string(int _Val)
    {
        ostringstream ss;
        ss << _Val;
        return ss.str();
    }
}
#endif

WDialogManager::WDialogManager(struct SDL_Window* win, struct SDL_Renderer* ren)
    : win(win), ren(ren)
{
    // initialize TTF_Init
    if(TTF_Init() == -1) {
        fprintf(stderr, "\nError initializing SDL2_ttf: %s\n", TTF_GetError());
        exit(1);
    }

    // load font
    main_font = TTF_OpenFont(DATAPREFIX "/PressStart2P.ttf", 16);
    small_font = TTF_OpenFont(DATAPREFIX "/PressStart2P.ttf", 8);
    if(!main_font || !small_font) {
        fprintf(stderr, "\nUnable to load font: %s\n", TTF_GetError());
        exit(1);
    }
}


WDialogManager::~WDialogManager()
{
    TTF_CloseFont(small_font);
    TTF_CloseFont(main_font);
    TTF_Quit();
}

void 
WDialogManager::Speech(class Person const& person, string const& message) const
{
    MessageBox_(person, message);

    ui->WaitForKeypress();
    ui->RedrawScene();
    ui->PresentScene();
}


void 
WDialogManager::Shopkeeper(class Shopkeeper& shopkeeper) const
{
    bool closed = false;
    map<Resource, SDL_Rect> mrects;
    vector<SDL_Rect> crects;
    while(!closed) {
        ShopKeeperDraw(shopkeeper, mrects, crects);
        closed = ShopKeeperEvents(shopkeeper, mrects, crects);
        crects.clear();
        mrects.clear();
    }
}


void 
WDialogManager::Banker(class Banker& banker) const
{
    Hero& hero = world->Hero();

    if(hero.LoanValue() > 0 
    && Question(banker, mprintf(_("You currently have a loan of $%d. Do you want to pay it now? [y/n]"), hero.LoanValue())) == "y") {
        int value = QuestionNumber(banker, mprintf(_("How much of your loan ($%d) do you want to pay?"), hero.LoanValue()), 8);
        if(value > hero.LoanValue()) {
            value = hero.LoanValue();
        }
        if(value > hero.Money()) {
            Speech(banker, _("You don't have enough money to pay the debt."));
        } else {
            hero.PayLoan(banker, value);
            Speech(banker, _("Very well."));
        }
    } else {
        // get loan
        int max_loan = banker.MaxLoanPossible(hero);
        if(max_loan <= 0) {
            Speech(banker, _("Unfortunately, your credit wasn't approved."));
        } else {
            int value = QuestionNumber(banker, 
                    mprintf(_("I can grant you a loan of up to $%d. How much do you want to apply for?"), max_loan), 8);
            if(value > max_loan) {
                Speech(banker, mprintf(_("Like I said, the maximum loan is %d."), max_loan));
            } else if (value > 0) {
                Speech(banker, _("Very well, here is your money."));
                banker.GiveLoan(hero, value);
            }
        }
    }
}


void 
WDialogManager::Bartender(class Bartender& bartender) const
{
    Speech(bartender, bartender.News());
}



/************************************************************************/


string 
WDialogManager::QuestionString(class Person const& person, string const& message, bool limit_to_numbers, unsigned int digits) const
{
    string reply;

    // get parameters
    int win_w, win_h;
    SDL_GetWindowSize(win, &win_w, &win_h);
    int h = TTF_FontLineSkip(main_font);

    // print message
    int y = MessageBox_(person, message + "\n");

    // draw edit text
    auto redraw_text = [&]() -> void {
        SDL_Rect r1{ 25, y, win_w-25, h };
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderFillRect(ren, &r1);
        WriteTextOnScreen(main_font, reply, 25, y, 255, 255, 255);
        SDL_RenderPresent(ren);
    };

    // input from user
    SDL_Event e;
    SDL_StartTextInput();
    for(;;) {
        while(SDL_PollEvent(&e)) {
            switch(e.type) {
            case SDL_TEXTINPUT:
                if(digits == 0 || reply.size() < digits) {
                    if(limit_to_numbers) {
                        if(e.text.text[0] < '0' || e.text.text[0] > '9') {
                            break;
                        }
                    }
                    reply.append(e.text.text);
                    redraw_text();
                }
                break;
            case SDL_KEYDOWN:
                if(e.key.keysym.sym == SDLK_BACKSPACE && reply.size() > 0) {
                    reply.resize(reply.size() - 1);
                    redraw_text();
                } else if(e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER) {
                    SDL_StopTextInput();
                    return reply;
                }
                break;
            default:
                ;
            }
        }
    }
}


int 
WDialogManager::MessageBox_(class Person const& person, string const& message) const
{
    (void) person;

    // get screen size
    int win_w, win_h;
    SDL_GetWindowSize(win, &win_w, &win_h);

    // find font size
    int advance;
    TTF_GlyphMetrics(main_font, 'A', NULL, NULL, NULL, NULL, &advance);
    
    vector<string> lines = Wrap(message, static_cast<unsigned int>((win_w-50) / advance));
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
	
    return y - TTF_FontLineSkip(main_font);
}


void 
WDialogManager::ShopKeeperDraw(class Shopkeeper& shopkeeper, 
        map<Resource, SDL_Rect>& mrects, vector<SDL_Rect>& crects) const
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
    WriteTextOnScreen(small_font, "Hero ($" + to_string(hero.Money()) + "):", 
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
        WriteTextOnScreen(main_font, string(1, static_cast<char>(res)), x+5, 175, 0, 0, 0);
        WriteTextOnScreen(small_font, to_string(shopkeeper.ResourceAmount(res)), x, 200, 0, 0, 0);
        auto s = mprintf("%d/%d", shopkeeper.ResourceBuyPrice(res), shopkeeper.ResourceSellPrice(res));
        WriteTextOnScreen(small_font, string(s), x, 212, 0, 0, 0);
        
        x += 40;
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
            WriteTextOnScreen(main_font, string(1, static_cast<char>(cs.Cargo)), x+5, 295, 0, 0, 0);
            WriteTextOnScreen(small_font, to_string(cs.Amount), x, 320, 0, 0, 0);
        }
        
        x += 35;
    }

	SDL_RenderPresent(ren);
}


bool
WDialogManager::ShopKeeperEvents(class Shopkeeper& shopkeeper, 
        map<Resource, SDL_Rect> const& mrects, vector<SDL_Rect> const& crects) const
{
    Resource dragging = Resource::NOTHING;
    unsigned int cargo_slot = UINT_MAX;
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
                    unsigned int slot = 0;
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
                                string message;
                                unsigned int amount = min(100U, shopkeeper.ResourceAmount(dragging));
                                if(SDL_GetModState() & KMOD_SHIFT) {
                                    string s = mprintf(_("How much %s do you want to buy?"), resource_name(dragging).c_str());
                                    amount = static_cast<unsigned int>(QuestionNumber(shopkeeper, s, 5));
                                }
                                shopkeeper.Sell(world->Hero(), dragging, amount, message);
                                if(!message.empty()) {
                                    cout << message << "\n";
                                }
                                return false;
                            }
                            ++slot;
                        }

                    // check if dropping on shopkeeper (selling)
                    } else if(action == SELLING) {
                        for(auto const& mrect: mrects) {
                            if(in_rect(e.button.x, e.button.y, mrect.second)) {
                                auto& slot = world->Hero().Vehicle().Cargo(cargo_slot);
                                if(slot.Cargo != Resource::NOTHING) {
                                    unsigned int amount = min(100U, slot.Amount);
                                    if(SDL_GetModState() & KMOD_SHIFT) {
                                        string s = mprintf(_("How much %s do you want to sell?"), resource_name(slot.Cargo).c_str());
                                        amount = static_cast<unsigned int>(QuestionNumber(shopkeeper, s, 5));
                                    }
                                    string message;
                                    shopkeeper.Buy(world->Hero(), slot.Cargo, amount, message);
                                    if(!message.empty()) {
                                        cout << message << "\n";
                                    }
                                    return false;
                                }
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
}


int
WDialogManager::WriteTextOnScreen(TTF_Font* font, string const& text, int x, int y, uint8_t r, uint8_t g, uint8_t b) const
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
