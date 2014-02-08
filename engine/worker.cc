#include "engine/worker.h"

#include <iostream>

#include "./globals.h"
#include "engine/city.h"
#include "ui/ui.h"

Worker::Worker(Point init, City& city, WorkerJob job)
    : Person(init), city(city), job(job)
{
}


Worker::~Worker()
{
}


void 
Worker::TalkToHero()
{
    switch(job) {
    case WorkerJob::SHOPKEEPER:
        ui->Dialog().Speech(*this, _("Welcome, sir! Please take a look at our fine merchindisings!"));
        ui->Dialog().Shopkeeper(city);
        break;
    default:
        abort();
    }
}


// vim: ts=4:sw=4:sts=4:expandtab
