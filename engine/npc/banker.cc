#include "engine/workers/banker.h"

#include <cassert>

#include "./globals.h"
#include "ui/ui.h"

Banker::Banker(Point init, class City& city)
    : Worker(init, city)
{
}

Banker::~Banker()
{
}


int 
Banker::Collateral(class Person const& person) const
{
    return 50000; // TODO
}


void
Banker::GiveLoan(class Person& person, int value) const
{
    assert(value + person.LoanValue() <= Collateral(person));
    person.setLoanValue(value);
}


void
Banker::TalkToHero()
{
    ui->Dialog().Banker(*this);
}

// vim: ts=4:sw=4:sts=4:expandtab
