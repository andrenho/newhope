#ifndef RENDER_H
#define RENDER_H

#include "lua.h"

void init_render();
int cb_render(lua_State* L);
int cb_render_physics(lua_State* L);
int cb_window_tiles(lua_State* L);

#endif
