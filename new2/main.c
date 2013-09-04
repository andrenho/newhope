#include "engine/world.h"
#include "ui/ui.h"

#include <stdlib.h>

int main()
{
	UI* ui;

	//
	// initialize everything
	//
	World* world = world_init(200, 200);
	if(!(ui = ui_init(world)))
		return EXIT_FAILURE;

	//
	// main loop
	//
	while(ui_active(ui)) {
		ui_do_events(ui);
		world_step(world);
		ui_render(ui);
		ui_wait_next_frame(ui);
	}

	//
	// free everything
	//
	ui_free(&ui);
	world_free(&world);
}
