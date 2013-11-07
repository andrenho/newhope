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
		ui_do_events();
		if_next_frame();
		ui_render();
	}

	// finish everything
	ui_finish();
	if_finish();
}
