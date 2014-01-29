#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <vector>
using namespace std;

#include "engine/command.h"
#include "engine/world.h"
#include "ui/ui.h"
#include "ui/w/wireframeui.h"

World* world;
UI* ui;

int main(int argc, char** argv)
{
	world = new World(-10000, -10000, 10000, 10000);
	ui = new WireframeUI();

	while(ui->Active()) {
		uint32_t next_frame = ui->Now() + 1000/60;

		// process keyboard
		vector<Command*> commands;
		ui->GetEvents(commands);
		for(auto& command : commands) {
			command->Execute();
			delete command;
		}

		// redraw screen
		ui->RedrawScene();

		// wait for next frame
		uint32_t now = ui->Now();
		printf("Frame time: %d\n", next_frame - now);
		if(now < next_frame) {
			ui->Wait(next_frame - now);
		}
	}

	delete ui;
	delete world;

	return EXIT_SUCCESS;
}
