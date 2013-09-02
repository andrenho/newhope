#include "engine/world.h"
#include "ui/ui.h"

#include <stdlib.h>

World* world;
UI*    ui;

int main()
{
	//
	// initialize everything
	//
	world = world_init(10, 10);
	if(!(ui = ui_init()))
		return EXIT_FAILURE;

	//
	// main loop
	//
	while(ui_active(ui)) {
		ui_do_events(ui, world);
		ui_render(ui, world);
	}

	//
	// free everything
	//
	ui_free(&ui);
	world_free(&world);
}
