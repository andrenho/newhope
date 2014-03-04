#include "engine/npc/cardealer.h"

#include <memory>
using namespace std;

#include "./main.h"
#include "ui/ui.h"
#include "ui/dialogmanager.h"

CarDealer::CarDealer(Point init, class City& city)
    : Worker(init, city, 0)
{
}

void
CarDealer::TalkToHero()
{
}


// vim: ts=4:sw=4:sts=4:expandtab
