// Copyright 2014 <Imperial Software>

#include "engine/command.h"

#include "./globals.h"
#include "ui/ui.h"

void
QuitCommand::Execute() const
{
	ui->Quit();
}


void
MoveCommand::Execute() const
{
	Point pos = world->Hero().Position();
	double x = pos.X(), y = pos.Y();

	if(up) { y -= 100; } else if(down) { y += 100; }
	if(left) { x -= 100; } else if(right) { x += 100; }
	world->Hero().SetTarget(Point(x,y));
}
