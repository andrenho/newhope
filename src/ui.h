#ifndef UI_H
#define UI_H

#include <lua.h>

int ui_c_init(lua_State* L);
int ui_now(lua_State* L);
int ui_wait(lua_State* L);
int ui_render(lua_State* L);
int ui_user_events(lua_State* L);
int ui_keyboard_state(lua_State* L);
int ui_visible_tiles(lua_State* L);
int ui_center_screen(lua_State* L);
int ui_message(lua_State* L);
int ui_clean_up(lua_State* L);

int ui_wrap_text(char* text, int chars);

#endif
