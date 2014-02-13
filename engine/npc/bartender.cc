#include "engine/workers/bartender.h"

#include "./globals.h"
#include "ui/ui.h"

Bartender::Bartender(Point init, class City& city)
    : Worker(init, city)
{
}

Bartender::~Bartender()
{
}


void
Bartender::TalkToHero()
{
}

// vim: ts=4:sw=4:sts=4:expandtab
