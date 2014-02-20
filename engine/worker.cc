#include "engine/worker.h"

#include <cstdlib>
using namespace std;


#include "./main.h"
#include "engine/city.h"
#include "engine/npc/banker.h"
#include "engine/npc/bartender.h"
#include "engine/npc/shopkeeper.h"

Worker::Worker(Point init, City& city, int money)
    : Person(init, money), city(city)
{
}


shared_ptr<Worker>
Worker::MakeWorker(Point init, class City& city, WorkerJob job)
{
    switch(job) {
    case WorkerJob::SHOPKEEPER:
        return shared_ptr<Worker>(new Shopkeeper(init, city));
    case WorkerJob::BANKER:
        return shared_ptr<Worker>(new Banker(init, city));
    case WorkerJob::BARTENDER:
        return shared_ptr<Worker>(new Bartender(init, city));
    default:
        abort();
    }
}


// vim: ts=4:sw=4:sts=4:expandtab
