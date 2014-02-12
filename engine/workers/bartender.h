#ifndef ENGINE_WORKERS_BARTENDER_H_
#define ENGINE_WORKERS_BARTENDER_H_

#include "engine/worker.h"
#include "engine/point.h"

class Bartender final : public Worker {
public:
    Bartender(Point init, class City& city);
    ~Bartender();

    void TalkToHero() override;

private:
    Bartender(const Bartender&);
    Bartender& operator=(const Bartender&);
};

#endif  // ENGINE_WORKERS_BARTENDER_H_

// vim: ts=4:sw=4:sts=4:expandtab
