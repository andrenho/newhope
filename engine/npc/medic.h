#ifndef ENGINE_NPC_MEDIC_H_
#define ENGINE_NPC_MEDIC_H_

#include <string>
using namespace std;

#include "engine/worker.h"
#include "engine/point.h"

class Medic final : public Worker {
public:
    Medic(Point init, class City& city);

    Medic(const Medic&) = delete;
    Medic& operator=(const Medic&) = delete;

    void TalkToHero() override;
};

#endif  // ENGINE_NPC_MEDIC_H_

// vim: ts=4:sw=4:sts=4:expandtab
