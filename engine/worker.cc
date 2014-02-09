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
        std::cout << ui->Dialog().Question(*this, _("Welcome, sir! Please take a look at our fine merchindisings!")) << std::endl;
        // ui->Dialog().Shopkeeper(city);
        break;
    default:
        ;
    }
}


// vim: ts=4:sw=4:sts=4:expandtab
