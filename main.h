#ifndef GLOBALS_H
#define GLOBALS_H

#include <libintl.h>
#include <memory>

#define _(STRING) gettext(STRING)

extern std::unique_ptr<class World> world;
extern std::unique_ptr<class UI> ui;

#endif


// vim: ts=4:sw=4:sts=4:expandtab
