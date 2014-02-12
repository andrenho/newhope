#ifndef ENGINE_WORKERS_BANKER_H_
#define ENGINE_WORKERS_BANKER_H_

#include "engine/worker.h"
#include "engine/point.h"

class Banker final : public Worker {
public:
    Banker(Point init, class City& city);
    ~Banker();

    void TalkToHero() override;

private:
    Banker(const Banker&);
    Banker& operator=(const Banker&);
};

#endif  // ENGINE_WORKERS_BANKER_H_

// vim: ts=4:sw=4:sts=4:expandtab
