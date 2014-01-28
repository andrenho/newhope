#ifndef DIALOG_H
#define DIALOG_H

#include <lua.h>

#include "ui.h"

int dialog_message(lua_State* L);
int dialog_buysell(lua_State* L);

#endif
