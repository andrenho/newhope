#ifndef GLOBALS_H
#define GLOBALS_H

#include <libintl.h>

#define _(STRING) gettext(STRING)

extern class World* world;
extern class UI* ui;

#endif


// vim: ts=4:sw=4:sts=4:expandtab
