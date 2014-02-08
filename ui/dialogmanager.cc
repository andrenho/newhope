#include "ui/dialogmanager.h"

std::vector<std::string>
DialogManager::Wrap(std::string text, int nchars)
{
    unsigned int ptr = 0, last_ptr = 0;
    std::vector<std::string> lines;

    for(;;) {
        ptr += nchars;
        if(ptr > text.length()) {
            lines.push_back(text.substr(last_ptr));
            return lines;
        }
        while(text[ptr] != ' ' && ptr > 0) {
            --ptr;
        }
        lines.push_back(text.substr(last_ptr, ptr));
        last_ptr = ptr + 1;
    }
}

// vim: ts=4:sw=4:sts=4:expandtab
