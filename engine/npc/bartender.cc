#include "engine/npc/bartender.h"

#include <memory>
using namespace std;

#include "./main.h"
#include "ui/ui.h"
#include "ui/dialogmanager.h"

Bartender::Bartender(Point init, class City& city)
    : Worker(init, city, 0)
{
}

Bartender::~Bartender()
{
}


void
Bartender::TalkToHero()
{
    ui->Dialog().Bartender(*this);
}


string 
Bartender::News() const
{
    return _("No news is good news.");
}


// vim: ts=4:sw=4:sts=4:expandtab
