#include "engine/workers/banker.h"

#include "./globals.h"
#include "ui/ui.h"

Banker::Banker(Point init, class City& city)
    : Worker(init, city)
{
}

Banker::~Banker()
{
}


void
Banker::TalkToHero()
{
    ui->Dialog().Banker(*this);
}

// vim: ts=4:sw=4:sts=4:expandtab
