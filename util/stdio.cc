#include "util/stdio.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
using namespace std;

#ifdef __MINGW32__
FILE *ftrash = NULL; 

int vasprintf( char **sptr, const char *fmt, va_list argv ) 
{ 
        if(!ftrash) ftrash = fopen("nul", "wb"); 
        if(!ftrash) fprintf(stderr, "this shouldn't happen\n"); 

        int wanted = vfprintf( ftrash, fmt, argv ); 

        if( (wanted < 0) || ((*sptr = (char*)malloc( 1 + wanted )) == NULL) ) 
        return -1; 

        (*sptr)[wanted] = '\0'; 
        return vsnprintf( *sptr, wanted, fmt, argv ); 
} 
#endif

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
