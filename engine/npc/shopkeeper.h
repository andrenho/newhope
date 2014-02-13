#ifndef ENGINE_WORKERS_SHOPKEEPER_H_
#define ENGINE_WORKERS_SHOPKEEPER_H_

#include "engine/worker.h"
#include "engine/point.h"

class Shopkeeper final : public Worker {
public:
    Shopkeeper(Point init, class City& city);
    ~Shopkeeper();

    void TalkToHero() override;

private:
    Shopkeeper(const Shopkeeper&);
    Shopkeeper& operator=(const Shopkeeper&);
};

#endif  // ENGINE_WORKERS_SHOPKEEPER_H_

// vim: ts=4:sw=4:sts=4:expandtab
