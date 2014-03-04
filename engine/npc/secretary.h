#ifndef ENGINE_NPC_SECRETARY_H_
#define ENGINE_NPC_SECRETARY_H_

#include <string>
using namespace std;

#include "engine/worker.h"
#include "engine/point.h"

class Secretary final : public Worker {
public:
    Secretary(Point init, class City& city);

    Secretary(const Secretary&) = delete;
    Secretary& operator=(const Secretary&) = delete;

    void TalkToHero() override;
};

#endif  // ENGINE_NPC_SECRETARY_H_

// vim: ts=4:sw=4:sts=4:expandtab
