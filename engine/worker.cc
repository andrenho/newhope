#include "engine/worker.h"

#include "engine/city.h"

Worker::Worker(Point init, City& city, WorkerJob job)
    : Person(init), city(city), job(job)
{
}

Worker::~Worker()
{
}


// vim: ts=4:sw=4:sts=4:expandtab
