#ifndef ENGINE_NPC_CARDEALER_H_
#define ENGINE_NPC_CARDEALER_H_

#include <string>
using namespace std;

#include "engine/worker.h"
#include "engine/point.h"

class CarDealer final : public Worker {
public:
    CarDealer(Point init, class City& city);

    CarDealer(const CarDealer&) = delete;
    CarDealer& operator=(const CarDealer&) = delete;

    void TalkToHero() override;
};

#endif  // ENGINE_NPC_CARDEALER_H_

// vim: ts=4:sw=4:sts=4:expandtab
