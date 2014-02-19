#ifndef ENGINE_PERSON_H_
#define ENGINE_PERSON_H_

#include <memory>
using namespace std;

#include "engine/object.h"
#include "engine/resources.h"

class Person : public Object {
public:
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
    struct cpBody *body, *target;
    struct cpShape* shape;
    struct cpConstraint* joint;
    shared_ptr<class Vehicle> vehicle;
    bool in_vehicle;
    int loan;
    unique_ptr<class Wallet> wallet;

private:
    Person(const Person&);
    Person& operator=(const Person&);
};


// this proxy class ensures that a person is only paid when the other one is giving money
class Wallet {
public:
    explicit Wallet(int money) : money(money) {}

    inline int Money() const { return money; }

    friend bool Person::Pay(Person& receiver, int value);
    friend void Person::GetPaid(Person& giver, int value);

private:
    int money;
};


#endif  // ENGINE_PERSON_H_


// vim: ts=4:sw=4:sts=4:expandtab
