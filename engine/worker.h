#ifndef ENGINE_WORKER_H_
#define ENGINE_WORKER_H_

#include "engine/person.h"

enum WorkerJob {
    SHOPKEEPER,
};

class Worker final : public Person {
public:
    Worker(Point init, class City& city, WorkerJob job);
    ~Worker();

    void TalkToHero() override;

private:
    Worker(const Worker&);
    Worker& operator=(const Worker&);

    class City& city;
    WorkerJob job;
};

#endif  // ENGINE_WORKER_H_


// vim: ts=4:sw=4:sts=4:expandtab
