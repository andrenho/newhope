#ifndef ENGINE_NPC_SHOPKEEPER_H_
#define ENGINE_NPC_SHOPKEEPER_H_

#include <map>
#include <string>
using namespace std;

#include "engine/worker.h"
#include "engine/point.h"
#include "engine/resources.h"

class Shopkeeper final : public Worker {
public:
    Shopkeeper(Point init, class City& city);

    Shopkeeper(const Shopkeeper&) = delete;
    Shopkeeper& operator=(const Shopkeeper&) = delete;

    inline unsigned int ResourceAmount(Resource r) const { return resources.at(r); }
    inline unsigned int ResourceSellPrice(Resource r) const { (void)r; return 5; }
    inline unsigned int ResourceBuyPrice(Resource r) const { (void)r; return 10; }

    bool Sell(class Person& person, Resource resource, unsigned int amount, string& message);
    bool Buy(class Person& person, Resource resource, unsigned int amount, string& message);

    void TalkToHero() override;

private:
    void ChangeCargoAmount(Resource res, unsigned int amount);

    map<Resource, unsigned int> resources = {};
};

#endif  // ENGINE_NPC_SHOPKEEPER_H_

// vim: ts=4:sw=4:sts=4:expandtab
