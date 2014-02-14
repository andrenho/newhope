#include "engine/worker.h"

#include <cstdlib>

#include "./main.h"
#include "engine/city.h"
#include "engine/npc/banker.h"
#include "engine/npc/bartender.h"
#include "engine/npc/shopkeeper.h"

Worker::Worker(Point init, City& city, int money)
    : Person(init, money), city(city)
{
}


Worker::~Worker()
{
}


std::shared_ptr<Worker>
Worker::MakeWorker(Point init, class City& city, WorkerJob job)
{
    switch(job) {
    case WorkerJob::SHOPKEEPER:
        return std::shared_ptr<Worker>(new Shopkeeper(init, city));
    case WorkerJob::BANKER:
        return std::shared_ptr<Worker>(new Banker(init, city));
    case WorkerJob::BARTENDER:
        return std::shared_ptr<Worker>(new Bartender(init, city));
    default:
        abort();
    }
}


// vim: ts=4:sw=4:sts=4:expandtab
