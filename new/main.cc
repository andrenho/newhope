#include "defines.h"

#include <cstdlib>
#include <iostream>
using namespace std;

Engine*  game;
UI*      ui;
Options* opt;

int main(int argc, char** argv)
{
	// find execution type
	opt = new Options(argc, argv);
	try {
		if(opt->Type == OPT_GAME) {
			game = new Game();
			ui =   new UIGame();
		} else if(opt->Type == OPT_EDITOR) {
			game = new Editor();
			ui =   new UIEditor();
		} else {
			abort();
		}
	} catch(ui_error const& e) {
		cerr << "Error initializing UI: " << e.what() << endl;
		exit(EXIT_FAILURE);
	}

	// initialize UI
	ui->Initialize();

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
