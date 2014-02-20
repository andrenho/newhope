#ifndef ENGINE_NPC_BANKER_H_
#define ENGINE_NPC_BANKER_H_

#include "engine/worker.h"
#include "engine/point.h"

class Banker final : public Worker {
public:
    Banker(Point init, class City& city);

    Banker(const Banker&) = delete;
    Banker& operator=(const Banker&) = delete;

    void TalkToHero() override;

    int MaxLoanPossible(class Person const& person) const { return Collateral(person) - person.LoanValue(); }
    int Collateral(class Person const& person) const;
    void GiveLoan(class Person& person, int value);
};

#endif  // ENGINE_NPC_BANKER_H_

// vim: ts=4:sw=4:sts=4:expandtab
