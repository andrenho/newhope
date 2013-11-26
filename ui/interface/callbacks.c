#include "callbacks.h"

#include <stdbool.h>

#include "interface.h"

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

CALLBACK callbacks;

extern lua_State *L;

static int if_lua_message_callback(lua_State *L) {
	// check parameters
	Message msg;
	msg.text = luaL_checkstring(L, 1);
	msg.type = luaL_checkint(L, 2);
	if(lua_type(L, 3) != LUA_TNIL)
		luaL_checktype(L, 3, LUA_TTABLE);
	if(lua_type(L, 4) != LUA_TNIL)
		msg.image = luaL_checkint(L, 4);
	if(lua_type(L, 5) != LUA_TNIL)
		msg.person_id = luaL_checkint(L, 5);
	lua_pop(L, 5);

	// find options
	msg.options[0] = NULL;

	// call callback
	MessageResponse r = callbacks.message(&msg);
	if(msg.type == MESSAGE)
		lua_pushinteger(L, r.option);
	else
		lua_pushstring(L, r.input);
	return 1;
}


void if_install_callbacks(MessageResponse (*message_cb)(Message*),
		void (*lua_error_cb)(const char*))
{
	callbacks.message = message_cb;
	callbacks.lua_error = lua_error_cb;

	check_stack();

	lua_getglobal(L, "callback");

	// message
	LUA_PUSH_METHOD("install");
	lua_pushstring(L, "message");
	lua_pushcfunction(L, if_lua_message_callback);
	LUA_CALL(3, 0);

	lua_pop(L, 1);

	check_stack();
}



