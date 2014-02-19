#include "util/stdio.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
using namespace std;

string mprintf(string fmt, ...)
{
    char* str;
    va_list ap;

    va_start(ap, fmt);
    vasprintf(&str, fmt.c_str(), ap);
    va_end(ap);

    string s = str;
    free(str);
    return s;
}

// vim: ts=4:sw=4:sts=4:expandtab
