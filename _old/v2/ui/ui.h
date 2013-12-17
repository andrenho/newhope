#ifndef UI_H
#define UI_H

#include "lua.h"

int cb_initialize_ui(lua_State* L);
int cb_active(lua_State* L);
int cb_current_time_ms(lua_State* L);
int cb_get_commands(lua_State* L);
int cb_sleep_ms(lua_State* L);
int cb_finish_ui(lua_State* L);

#endif
