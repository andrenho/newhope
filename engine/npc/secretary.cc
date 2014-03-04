#include "engine/npc/secretary.h"

#include <memory>
using namespace std;

#include "./main.h"
#include "ui/ui.h"
#include "ui/dialogmanager.h"

Secretary::Secretary(Point init, class City& city)
    : Worker(init, city, 0)
{
}

void
Secretary::TalkToHero()
{
}


// vim: ts=4:sw=4:sts=4:expandtab
