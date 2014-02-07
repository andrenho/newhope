#ifndef UI_H
#define UI_H

#include <stdbool.h>
#include <stdint.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#include "resources.h"
#include "background.h"

struct World;

#define TILE_W 16
#define TILE_H 16

#define FPS 60

typedef struct UI {
    SDL_Window* win;
    SDL_Renderer* ren;
    bool active;
    int rx, ry;
    uint32_t last_frame;
    TTF_Font* font;
} UI;

bool ui_init();
void ui_finish();

bool ui_active();
void ui_do_events();
void ui_render();
void ui_wait_next_frame();
void ui_lua_error(const char* msg);

MessageResponse ui_show_message(Message* msg);

void ui_screen_limits(int* x1, int* y1, int* x2, int* y2);

#endif


// vim: ts=4:sw=4:sts=4:expandtab
