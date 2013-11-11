#include "interface.h"
#include "ui.h"

#include <stdint.h>
#include <stdio.h>

int main()
{
	// initialize everything
	if_init();
	ui_init();

	// main loop
	while(ui_active()) {
		Message* msg = if_message_pending();
		if(msg) {
			ui_show_message(msg);
			MessageResponse r = ui_respond_message(msg);
			if_respond_message(r);
			if_free_message(&msg);
			exit(2);
		} else {
			ui_do_events();
			if_next_frame();
			ui_render();
		}
	}

	// finish everything
	ui_finish();
	if_finish();
}
