#ifndef ENGINE_NPC_SMUGGLER_H_
#define ENGINE_NPC_SMUGGLER_H_

#include <string>
using namespace std;

#include "engine/worker.h"
#include "engine/point.h"

class Smuggler final : public Worker {
public:
    Smuggler(Point init, class City& city);

    Smuggler(const Smuggler&) = delete;
    Smuggler& operator=(const Smuggler&) = delete;

    void TalkToHero() override;
};

#endif  // ENGINE_NPC_SMUGGLER_H_

// vim: ts=4:sw=4:sts=4:expandtab
