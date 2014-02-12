#include "engine/worker.h"

#include <cstdlib>

#include "./globals.h"
#include "engine/city.h"
#include "engine/workers/banker.h"
#include "engine/workers/shopkeeper.h"

Worker::Worker(Point init, City& city)
    : Person(init), city(city)
{
}


Worker::~Worker()
{
}


Worker* 
Worker::MakeWorker(Point init, class City& city, WorkerJob job)
{
    switch(job) {
    case WorkerJob::SHOPKEEPER:
        return new Shopkeeper(init, city);
    case WorkerJob::BANKER:
        return new Banker(init, city);
    default:
        abort();
    }
}


// vim: ts=4:sw=4:sts=4:expandtab
