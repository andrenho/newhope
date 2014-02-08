#include "engine/command.h"

#include "./globals.h"
#include "engine/hero.h"
#include "engine/vehicle.h"
#include "ui/ui.h"

void
QuitCommand::Execute() const
{
    ui->Quit();
}


void
MoveCommand::Execute() const
{
    if(!world->Hero().InVehicle()) {
        Point pos = world->Hero().Position();
        double x = pos.X(), y = pos.Y();
        if(up) { y -= 100; } else if(down) { y += 100; }
        if(left) { x -= 100; } else if(right) { x += 100; }
        world->Hero().SetTarget(Point(x,y));
    } else {
        Vehicle& v = *world->Hero().Vehicle();
        v.Steering.accelerate = up;
        v.Steering.reverse = down;
        if(left) {
            v.Steering.wheel = -1;
        } else if(right) {
            v.Steering.wheel = 1;
        } else {
            v.Steering.wheel = 0;
        }
    }
}


void
ExitVehicleCommand::Execute() const
{
    world->Hero().ExitVehicle();
}


void
ShowMinimapCommand::Execute() const
{
    ui->ShowMinimap();
}


// vim: ts=4:sw=4:sts=4:expandtab
