#ifndef ENGINE_NPC_BARTENDER_H_
#define ENGINE_NPC_BARTENDER_H_

#include <string>
using namespace std;

#include "engine/worker.h"
#include "engine/point.h"

class Bartender final : public Worker {
public:
    Bartender(Point init, class City& city);

    Bartender(const Bartender&) = delete;
    Bartender& operator=(const Bartender&) = delete;

    void TalkToHero() override;
    string News() const;
};

#endif  // ENGINE_NPC_BARTENDER_H_

// vim: ts=4:sw=4:sts=4:expandtab
