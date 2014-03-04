#ifndef ENGINE_NPC_ORDINARY_H_
#define ENGINE_NPC_ORDINARY_H_

#include <string>
using namespace std;

#include "engine/worker.h"
#include "engine/point.h"

class Ordinary final : public Worker {
public:
    Ordinary(Point init, class City& city);

    Ordinary(const Ordinary&) = delete;
    Ordinary& operator=(const Ordinary&) = delete;

    void TalkToHero() override;
};

#endif  // ENGINE_NPC_ORDINARY_H_

// vim: ts=4:sw=4:sts=4:expandtab
