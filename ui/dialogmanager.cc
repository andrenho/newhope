#include "ui/dialogmanager.h"

#include <glog/logging.h>

std::vector<std::string>
DialogManager::Wrap(std::string const& text, unsigned int nchars)
{
    unsigned int ptr = 0, last_ptr = 0;
    std::vector<std::string> lines;

    for(;;) {
        ptr += nchars;

        // check for 'enters', and break it
        std::string::size_type pos;
        if((pos = text.substr(last_ptr, ptr).find('\n')) != std::string::npos) {
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

std::string 
DialogManager::Question(class Person const& person, std::string const& message) const
{
    return QuestionString(person, message, false, 0);
}


int 
DialogManager::QuestionNumber(class Person const& person, std::string const& message, unsigned int digits) const
{
    try {
        return std::stoi(QuestionString(person, message, true, digits));
    } catch(std::exception& e) {
        LOG(WARNING) << "Exception caught: '" << e.what() << "'\n";
        return 0;
    }
}


// vim: ts=4:sw=4:sts=4:expandtab
