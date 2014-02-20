#ifndef ENGINE_PERSON_H_
#define ENGINE_PERSON_H_

#include <memory>
using namespace std;

#include "engine/object.h"
#include "engine/resources.h"

class Person : public Object {
public:
    Person(const Person&) = delete;
    Person& operator=(const Person&) = delete;

    virtual ~Person();

    virtual void SetTarget(Point const& p);
    virtual void SetPosition(Point const& p);

    virtual void TalkToHero() {}

    inline bool InVehicle() const { return in_vehicle; }
    inline class Vehicle& Vehicle() const { return *vehicle; }
    bool ExitVehicle();

    inline virtual double Radius() const { return 0.5; }
    virtual Point Position() const override;
    inline struct cpBody* PhysicsBodyPtr() const { return body; }

    int Money() const;

    void GetPaid(Person& giver, int value); // TODO - should be private
    bool Pay(Person& receiver, int value);

    int LoanValue() const { return loan; }
    bool PayLoan(class Banker& banker, int value);

    void setLoanValue(Banker& banker, int value);

protected:
    Person(Point init, int money);

    const Point init;
    struct cpBody *body = nullptr, *target = nullptr;
    struct cpShape* shape = nullptr;
    struct cpConstraint* joint = nullptr;
    shared_ptr<class Vehicle> vehicle = nullptr;
    bool in_vehicle = false;
    int loan = 0;
    unique_ptr<class Wallet> wallet;
};


// this proxy class ensures that a person is only paid when the other one is giving money
class Wallet final {
public:
    explicit Wallet(int money) : money(money) {}

    Wallet(const Wallet&) = delete;
    Wallet& operator=(const Wallet&) = delete;

    inline int Money() const { return money; }

    friend bool Person::Pay(Person& receiver, int value);
    friend void Person::GetPaid(Person& giver, int value);

private:
    int money;
};


#endif  // ENGINE_PERSON_H_


// vim: ts=4:sw=4:sts=4:expandtab
