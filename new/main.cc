#include "defines.h"

Game*  game;
UI*    ui;

int main()
{
	game = new Game(20, 20);
	ui   = new UI();

	while(ui->Active()) {
		ui->ProcessInputs();
		ui->Render();
		ui->WaitNextFrame();
	}

	delete ui;
	delete game;
}
