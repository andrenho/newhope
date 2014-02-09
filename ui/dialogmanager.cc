#include "ui/dialogmanager.h"

std::vector<std::string>
DialogManager::Wrap(std::string text, unsigned int nchars)
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

// vim: ts=4:sw=4:sts=4:expandtab
