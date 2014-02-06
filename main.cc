// Copyright 2014 <Imperial Software>

#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <vector>

#include "engine/command.h"
#include "engine/world.h"
#include "ui/ui.h"
#include "ui/w/wireframeui.h"

World* world = nullptr;
UI*    ui    = nullptr;

int main(int argc, char** argv)
{
	(void) argc; (void) argv;

	unsigned int seedp = 0;

	ui = new WireframeUI();
	world = new World(-10000, -10000, 10000, 10000, seedp);

	world->Initialize();
	ui->Initialize();

	while(ui->Active()) {
		uint32_t next_frame = ui->Now() + static_cast<int>(1000.0/60.0);

		// process keyboard
		std::vector<Command*> commands;
		ui->GetEvents(commands);
		for(auto const& command : commands) {
			command->Execute();
			delete command;
		}

		// advance time
		world->Step();
		//printf("THI: %d    ", next_frame - ui->Now());

		// redraw screen
		ui->RedrawScene();
		//printf("RED: %d    ", next_frame - ui->Now());
		ui->PresentScene();

		// wait for next frame
		uint32_t now = ui->Now();
		//printf("PRE: %d\n", next_frame - now);
		if(now < next_frame) {
			ui->Wait(next_frame - now);
		}
	}

	delete world;
	delete ui;

	return EXIT_SUCCESS;
}
