#ifndef UI_DIALOGMANAGER_H_
#define UI_DIALOGMANAGER_H_

#include <string>
#include <vector>

class DialogManager {
public:
    DialogManager() {}
    virtual ~DialogManager() {}

    virtual void Speech(class Person const& person, std::string const& message) const = 0;
    std::string Question(class Person const& person, std::string const& message) const;
    int QuestionNumber(class Person const& person, std::string const& message, unsigned int digits) const;

    virtual void Shopkeeper(class City& city) const = 0;

protected:
    virtual std::string Question(class Person const& person, std::string const& message, bool limit_to_numbers, 
            unsigned int digits) const = 0;
    static std::vector<std::string> Wrap(std::string const& text, unsigned int nchars);

private:
    DialogManager(const DialogManager&);
    DialogManager& operator=(const DialogManager&);
};

#endif  // UI_DIALOGMANAGER_H_

// vim: ts=4:sw=4:sts=4:expandtab
