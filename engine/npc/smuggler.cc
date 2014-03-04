#include "engine/npc/smuggler.h"

#include <memory>
using namespace std;

#include "./main.h"
#include "ui/ui.h"
#include "ui/dialogmanager.h"

Smuggler::Smuggler(Point init, class City& city)
    : Worker(init, city, 0)
{
}

void
Smuggler::TalkToHero()
{
}


// vim: ts=4:sw=4:sts=4:expandtab
