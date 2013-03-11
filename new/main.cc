#include "defines.h"

#include <cstdlib>
#include <iostream>
using namespace std;

GameMode* game;
UI*       ui;
Options*  opt;

int main(int argc, char** argv)
{
	// find execution type and initialize UI
	opt = new Options(argc, argv);
	try {
		if(opt->Type == OPT_GAME) {
			game = new Game(20, 20);
			ui = new UIGame();
		} else if(opt->Type == OPT_EDITOR) {
			game = new Editor();
			ui = new UIEditor();
		} else {
			abort();
		}
	} catch(ui_error const& e) {
		cerr << "Error initializing UI: " << e.what() << endl;
		exit(EXIT_FAILURE);
	}

	// main loop
	while(ui->Active()) {
		ui->StartFrame();
		ui->ProcessInputs();
		if(ui->Active()) {
			ui->Render();
			ui->WaitNextFrame();
		}
	}

	// cleanup
	delete ui;
	delete game;
	delete opt;
}
