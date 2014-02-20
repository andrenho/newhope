#ifndef UI_UI_H_
#define UI_UI_H_

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
using namespace std;

#include "engine/rectangle.h"
#include "ui/dialogmanager.h"
#include "ui/minimap.h"

class Command;

class UI {
public:
    virtual ~UI() = default;

    UI(const UI&) = delete;
    UI& operator=(const UI&) = delete;

    virtual void Initialize() = 0;
    virtual bool Active() const = 0;
    virtual void Quit() = 0;

    virtual uint32_t Now() const = 0;
    virtual void Wait(uint32_t tm) const = 0;

    virtual vector<unique_ptr<Command>> GetEvents() const = 0;

    virtual Rectangle_& GetVisibleArea(Rectangle_& r) const = 0;
    virtual void RedrawScene() const = 0;
    virtual void PresentScene() const = 0;

    virtual void ShowMinimap() const = 0;

    inline DialogManager const& Dialog() const { return *dialog; }

    virtual void WaitForKeypress() const = 0;

protected:
    UI() {}

    unique_ptr<Minimap> minimap = nullptr;
    unique_ptr<DialogManager> dialog = nullptr;
};

#endif  // UI_UI_H_


// vim: ts=4:sw=4:sts=4:expandtab
