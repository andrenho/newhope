#include "ui/dialogmanager.h"

#include <cstdlib>
#include <string>
using namespace std;

vector<string>
DialogManager::Wrap(string const& text, unsigned int nchars)
{
    unsigned int ptr = 0, last_ptr = 0;
    vector<string> lines;

    for(;;) {
        ptr += nchars;

        // check for 'enters', and break it
        string::size_type pos;
        if((pos = text.substr(last_ptr, ptr).find('\n')) != string::npos) {
            lines.push_back(text.substr(last_ptr, pos));
            last_ptr = ptr = last_ptr + static_cast<int>(pos)+1;
            continue;
        }

        // if less than `nchars` left, add it and end it
        if(ptr >= text.length()) {
            lines.push_back(text.substr(last_ptr));
            return lines;
        }

        // advance `nchars` and go back until a space is found
        while(text[ptr] != ' ' && ptr > 0) {
            --ptr;
        }
        lines.push_back(text.substr(last_ptr, ptr));
        last_ptr = ptr + 1;
    }
}

string 
DialogManager::Question(class Person const& person, string const& message) const
{
    return QuestionString(person, message, false, 0);
}


int 
DialogManager::QuestionNumber(class Person const& person, string const& message, unsigned int digits) const
{
    try {
        return strtol(QuestionString(person, message, true, digits).c_str(), 0, 10);
    } catch(exception& e) {
        return 0;
    }
}


// vim: ts=4:sw=4:sts=4:expandtab
