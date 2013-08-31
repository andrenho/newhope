#include "global.h"

World* world;
UI*    ui;

int main()
{
	//
	// initialize everything
	//
	world = world_init();
	ui = ui_init();

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
