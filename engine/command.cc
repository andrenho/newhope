#include "command.h"

#include "globals.h"
#include "ui/ui.h"

void
QuitCommand::Execute()
{
	ui->Quit();
}
