#ifndef UI_DIALOGMANAGER_H_
#define UI_DIALOGMANAGER_H_

#include <string>
#include <vector>

class DialogManager {
public:
    DialogManager() {}
    virtual ~DialogManager() {}

    virtual void Speech(class Person const& person, std::string message) const = 0;
    virtual void Shopkeeper(class City& city) const = 0;

protected:
    static std::vector<std::string> Wrap(std::string text, int nchars);

private:
    DialogManager(const DialogManager&);
    DialogManager& operator=(const DialogManager&);
};

#endif  // UI_DIALOGMANAGER_H_

// vim: ts=4:sw=4:sts=4:expandtab
