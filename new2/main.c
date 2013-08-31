#include "global.h"

#include <stdlib.h>

World* world;
UI*    ui;

int main()
{
	//
	// initialize everything
	//
	world = world_init();
	if(!(ui = ui_init()))
		return EXIT_FAILURE;

	//
	// main loop
	//
	while(ui_active()) {
		ui_do_events();
		ui_render();
	}

	//
	// free everything
	//
	ui_free(&ui);
	world_free(&world);
}
