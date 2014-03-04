#ifndef ENGINE_NPC_DISPATCHER_H_
#define ENGINE_NPC_DISPATCHER_H_

#include <string>
using namespace std;

#include "engine/worker.h"
#include "engine/point.h"

class Dispatcher final : public Worker {
public:
    Dispatcher(Point init, class City& city);

    Dispatcher(const Dispatcher&) = delete;
    Dispatcher& operator=(const Dispatcher&) = delete;

    void TalkToHero() override;
};

#endif  // ENGINE_NPC_DISPATCHER_H_

// vim: ts=4:sw=4:sts=4:expandtab
