#ifndef UI_DIALOGMANAGER_H_
#define UI_DIALOGMANAGER_H_

#include <string>
#include <vector>
using namespace std;

class DialogManager {
public:
    virtual ~DialogManager() = default;

    DialogManager(const DialogManager&) = delete;
    DialogManager& operator=(const DialogManager&) = delete;

    virtual void Speech(class Person const& person, string const& message) const = 0;
    virtual string Question(class Person const& person, string const& message) const;
    int QuestionNumber(class Person const& person, string const& message, unsigned int digits) const;

    // workers dialogs
    virtual void Banker(class Banker& banker) const = 0;
    virtual void Bartender(class Bartender& bartender) const = 0;
    virtual void Shopkeeper(class Shopkeeper& shopkeeper) const = 0;

protected:
    DialogManager() {}

    virtual string QuestionString(class Person const& person, string const& message, bool limit_to_numbers, 
            unsigned int digits) const = 0;
    static vector<string> Wrap(string const& text, unsigned int nchars);
};

#endif  // UI_DIALOGMANAGER_H_

// vim: ts=4:sw=4:sts=4:expandtab
