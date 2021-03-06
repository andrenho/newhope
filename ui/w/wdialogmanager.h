#ifndef UI_W_WDIALOGMANAGER_H_
#define UI_W_WDIALOGMANAGER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <cstdint>
#include <map>
#include <string>
#include <vector>
using namespace std;

#include "ui/dialogmanager.h"
#include "engine/resources.h"

class WDialogManager final : public DialogManager {
public:
    WDialogManager(struct SDL_Window* win, struct SDL_Renderer* ren);
    ~WDialogManager();

    WDialogManager(const WDialogManager&) = delete;
    WDialogManager& operator=(const WDialogManager&) = delete;

    void Speech(class Person const& person, string const& message) const override;

    // workers
    void Banker(class Banker& banker) const override;
    void Bartender(class Bartender& bartender) const override;
    void Shopkeeper(class Shopkeeper& shopkeeper) const override;

protected:
    string QuestionString(class Person const& person, string const& message, bool limit_to_numbers, 
            unsigned int digits) const override;

private:
    int MessageBox_(class Person const& person, string const& message) const;

    void ShopKeeperDraw(class Shopkeeper& shopkeeper, map<Resource, SDL_Rect>& mrects, vector<SDL_Rect>& crects) const;
    bool ShopKeeperEvents(class Shopkeeper& shopkeeper, 
            map<Resource, SDL_Rect> const& mrects, vector<SDL_Rect> const& crects) const;

    int WriteTextOnScreen(TTF_Font* font, string const& text, int x, int y, 
            uint8_t r, uint8_t g, uint8_t b) const;

    struct SDL_Window* win;
    struct SDL_Renderer* ren;
    TTF_Font *small_font = nullptr, *main_font = nullptr;
};

#endif  // UI_W_WDIALOGMANAGER_H_

// vim: ts=4:sw=4:sts=4:expandtab
