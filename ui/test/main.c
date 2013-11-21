#include "interface.h"
#include "ui.h"

#include <stdint.h>
#include <stdio.h>

bool reload = false;

int main()
{
	// initialize everything
	ui_init();
load_lua:
	if_init();
	if_install_callbacks(ui_show_message);
	if_init_world(30, 30);

	// main loop
	while(ui_active()) {
		ui_do_events();
		if(reload) {
			reload = false;
			goto load_lua;
		}
		if_next_frame();
		ui_render();
	}

	// finish everything
	ui_finish();
	if_finish();
}
