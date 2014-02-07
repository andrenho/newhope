#ifndef ENGINE_COMMAND_H_
#define ENGINE_COMMAND_H_

#include "engine/point.h"
#include "engine/world.h"
#include "engine/hero.h"

class Command {
public:
    Command() { }
    virtual ~Command() { }

    virtual void Execute() const = 0;
private:
    Command(const Command&);
    Command& operator=(const Command&);
};


class QuitCommand final : public Command {
public:
    void Execute() const;
};


class MoveCommand final : public Command {
public:
    MoveCommand(bool up, bool down, bool left, bool right) : 
        up(up), down(down), left(left), right(right) { }
    void Execute() const;
private:
    bool up, down, left, right;
};


class ShowMinimapCommand final : public Command {
public:
    void Execute() const;
};

#endif  // ENGINE_COMMAND_H_


// vim: ts=4:sw=4:sts=4:expandtab
