#ifndef ENGINE_WORKER_H_
#define ENGINE_WORKER_H_

#include "engine/person.h"

enum WorkerJob {
    SHOPKEEPER, BANKER, CAR_DEALER, BARTENDER, MEDIC, SECRETARY, DISPATCHER/*?*/,
};

class Worker : public Person {
public:
    static Worker* MakeWorker(Point init, class City& city, WorkerJob job);
    virtual ~Worker();

protected:
    Worker(Point init, class City& city, int money);

    class City& city;

private:
    Worker(const Worker&);
    Worker& operator=(const Worker&);
};

#endif  // ENGINE_WORKER_H_


// vim: ts=4:sw=4:sts=4:expandtab
