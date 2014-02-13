#ifndef ENGINE_NPC_SHOPKEEPER_H_
#define ENGINE_NPC_SHOPKEEPER_H_

#include <map>
#include <string>

#include "engine/worker.h"
#include "engine/point.h"
#include "engine/resources.h"

class Shopkeeper final : public Worker {
public:
    Shopkeeper(Point init, class City& city);
    ~Shopkeeper();

    inline unsigned int ResourceAmount(Resource r) const { return resources.at(r); }
    inline unsigned int ResourceSellPrice(Resource r) const { (void)r; return 5; }
    inline unsigned int ResourceBuyPrice(Resource r) const { (void)r; return 10; }

    bool Sell(class Person& person, Resource resource, unsigned int amount, std::string& message);
    bool Buy(class Person& person, Resource resource, unsigned int amount, std::string& message);

    void TalkToHero() override;

private:
    Shopkeeper(const Shopkeeper&);
    Shopkeeper& operator=(const Shopkeeper&);

    void ChangeCargoAmount(Resource res, int amount);

    std::map<Resource, int> resources;
};

#endif  // ENGINE_NPC_SHOPKEEPER_H_

// vim: ts=4:sw=4:sts=4:expandtab
