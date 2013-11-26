#include "interface.h"
#include "timer.h"
#include "ui.h"

#include <stdint.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	// initialize UI
	ui_init();
load_lua:
	// initialize interface with LUA
	if_init();
	if_install_callbacks(ui_show_message, ui_lua_error);
	if_init_world(30, 30);

	// initialize timer
	Timer* t = timer_init(argc, argv);

	// main loop
	while(ui_active()) {
		ui_do_events();
		if(!ui_active()) {
			break;
		}
		timer_start(t);
		    if_next_frame();
		timer_stop(t);
		if(if_reload_engine) {
			goto load_lua;
		}
		ui_render();
	}

	// finish everything
	timer_finish(t);
	ui_finish();
	if_finish();
}
