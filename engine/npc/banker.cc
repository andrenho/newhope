#include "engine/npc/banker.h"

#include <cassert>
#include <climits>
using namespace std;

#include "./main.h"
#include "ui/ui.h"

Banker::Banker(Point init, class City& city)
    : Worker(init, city, INT_MAX/2)
{
}

Banker::~Banker()
{
}


int 
Banker::Collateral(class Person const& person) const
{
    (void) person;
    return 50000; // TODO
}


void
Banker::GiveLoan(class Person& person, int value)
{
    assert(value + person.LoanValue() <= Collateral(person));
    Pay(person, value);
    person.setLoanValue(*this, value);
}


void
Banker::TalkToHero()
{
    ui->Dialog().Banker(*this);
}

// vim: ts=4:sw=4:sts=4:expandtab
