#ifndef ENGINE_WORKER_H_
#define ENGINE_WORKER_H_

#include <memory>
using namespace std;

#include "engine/person.h"

enum WorkerJob {
    SHOPKEEPER, BANKER, CAR_DEALER, BARTENDER, MEDIC, SECRETARY, DISPATCHER, /*?*/
};

class Worker : public Person {
public:
    static shared_ptr<Worker> MakeWorker(Point init, class City& city, WorkerJob job);

    Worker(const Worker&) = delete;
    Worker& operator=(const Worker&) = delete;

protected:
    Worker(Point init, class City& city, int money);

    class City& city;
};

#endif  // ENGINE_WORKER_H_


// vim: ts=4:sw=4:sts=4:expandtab
