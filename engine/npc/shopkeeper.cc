#include "engine/workers/shopkeeper.h"

#include "./globals.h"
#include "ui/ui.h"

Shopkeeper::Shopkeeper(Point init, class City& city)
    : Worker(init, city)
{
}

Shopkeeper::~Shopkeeper()
{
}


void
Shopkeeper::TalkToHero()
{
    ui->Dialog().Shopkeeper(city, *this);
}

// vim: ts=4:sw=4:sts=4:expandtab
