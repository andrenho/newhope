#ifndef UI_H
#define UI_H

#include <lua.h>

int ui_c_init(lua_State* L);
int ui_now(lua_State* L);
int ui_wait(lua_State* L);
int ui_c_redraw(lua_State* L);
int ui_user_events(lua_State* L);
int ui_clean_up(lua_State* L);

#endif
