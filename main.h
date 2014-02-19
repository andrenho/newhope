#ifndef MAIN_H_
#define MAIN_H_

#include <libintl.h>
#include <memory>
using namespace std;

#define _(STRING) gettext(STRING)

extern unique_ptr<class World> world;
extern unique_ptr<class UI> ui;
extern struct cpSpace* space;

#endif  // MAIN_H_


// vim: ts=4:sw=4:sts=4:expandtab
