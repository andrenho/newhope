// Copyright 2014 <Imperial Software>

#include "engine/command.h"

#include "./globals.h"
#include "ui/ui.h"

void
QuitCommand::Execute() const
{
	ui->Quit();
}
