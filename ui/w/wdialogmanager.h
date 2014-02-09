#ifndef UI_W_WDIALOGMANAGER_H_
#define UI_W_WDIALOGMANAGER_H_

#include "engine/resources.h"
#include "ui/dialogmanager.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstdint>

#include <map>
#include <string>
#include <vector>

class WDialogManager final : public DialogManager {
public:
    WDialogManager(struct SDL_Window* win, struct SDL_Renderer* ren);
    ~WDialogManager();

    void Speech(class Person const& person, std::string message) const;
    std::string Question(class Person const& person, std::string message) const;
    void Shopkeeper(class City& city) const;

private:
    WDialogManager(const WDialogManager&);
    WDialogManager& operator=(const WDialogManager&);

    int MessageBox(class Person const& person, std::string message) const;

    void ShopKeeperDraw(class City& city, std::map<Resource, SDL_Rect>& mrects, std::vector<SDL_Rect>& crects) const;
    bool ShopKeeperEvents(class City& city, std::map<Resource, SDL_Rect> const& mrects, std::vector<SDL_Rect> const& crects) const;

    int WriteTextOnScreen(TTF_Font* font, std::string text, int x, int y, 
            uint8_t r, uint8_t g, uint8_t b) const;

    struct SDL_Window* win;
    struct SDL_Renderer* ren;
    TTF_Font *small_font, *main_font;
};

#endif  // UI_W_WDIALOGMANAGER_H_

// vim: ts=4:sw=4:sts=4:expandtab
