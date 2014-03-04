#include "engine/npc/ordinary.h"

#include <memory>
using namespace std;

#include "./main.h"
#include "ui/ui.h"
#include "ui/dialogmanager.h"

Ordinary::Ordinary(Point init, class City& city)
    : Worker(init, city, 0)
{
}

void
Ordinary::TalkToHero()
{
}


// vim: ts=4:sw=4:sts=4:expandtab
