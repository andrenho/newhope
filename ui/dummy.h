#ifndef UI_DUMMY_H_
#define UI_DUMMY_H_

#include "ui/dialogmanager.h"
#include "ui/ui.h"

class DummyDialogManager : public DialogManager {
public:
    void Speech(class Person const& person, std::string const& message) const override { }
    void Shopkeeper(class City& city) const override { }

protected:
    std::string Question(class Person const& person, std::string const& message, bool limit_to_numbers, 
            unsigned int digits) const override { return ""; }
};


class DummyUI : public UI {
public:
    DummyUI() : UI(), active(true) {}
    void Initialize() override { }
    bool Active() const override { return active; }
    void Quit() override { active = false; }

    uint32_t Now() const override { return 0; }
    void Wait(uint32_t tm) const override { }

    void GetEvents(std::vector<Command*>& commands) const override { commands.push_back(new QuitCommand()); }

    Rectangle& GetVisibleArea(Rectangle& r) const override { r.setP1(Point(0, 0)); r.setP2(Point(100, 100)); return r; }
    void RedrawScene() const override { }
    void PresentScene() const override { }

    void ShowMinimap() const override { }

    void WaitForKeypress() const override { }

private:
    bool active;
};

#endif  // UI_DUMMY_H_

// vim: ts=4:sw=4:sts=4:expandtab
