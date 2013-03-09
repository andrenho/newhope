#include "defines.h"

#include <cstdlib>
#include <iostream>
using namespace std;

Game*  game;
UI*    ui;

int main()
{
	game = new Game(20, 20);
	try {
		ui = new UI();
	} catch(glfw_error const& e) {
		cerr << "Error initializing UI: " << e.what() << endl;
		exit(EXIT_FAILURE);
	}

	while(ui->Active()) {
		ui->ProcessInputs();
		ui->Render();
		ui->WaitNextFrame();
	}

	delete ui;
	delete game;
}
