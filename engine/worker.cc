#include "engine/worker.h"

#include <cstdlib>
using namespace std;


#include "./main.h"
#include "engine/city.h"
#include "engine/npc/banker.h"
#include "engine/npc/bartender.h"
#include "engine/npc/cardealer.h"
#include "engine/npc/medic.h"
#include "engine/npc/secretary.h"
#include "engine/npc/dispatcher.h"
#include "engine/npc/shopkeeper.h"
#include "engine/npc/ordinary.h"
#include "engine/npc/smuggler.h"

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
    case WorkerJob::CAR_DEALER:
        return shared_ptr<Worker>(new CarDealer(init, city));
    case WorkerJob::MEDIC:
        return shared_ptr<Worker>(new Medic(init, city));
    case WorkerJob::DISPATCHER:
        return shared_ptr<Worker>(new Dispatcher(init, city));
    case WorkerJob::SECRETARY:
        return shared_ptr<Worker>(new Secretary(init, city));
    case WorkerJob::ORDINARY:
        return shared_ptr<Worker>(new Ordinary(init, city));
    case WorkerJob::SMUGGLER:
        return shared_ptr<Worker>(new Smuggler(init, city));
    default:
        abort();
    }
}


// vim: ts=4:sw=4:sts=4:expandtab
