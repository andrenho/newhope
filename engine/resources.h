#ifndef ENGINE_RESOURCES_H_
#define ENGINE_RESOURCES_H_

#include <array>
#include <string>
using namespace std;

enum Resource : char {
   GRAIN      = 'G', 
   WOOD       = 'W', 
   OIL        = 'O', 
   HERBS      = 'H', 
   SILICON    = 'S', 
   IRON       = 'I', 
   URANIUM    = 'U', 
   ELECRONICS = 'E',
   MACHINES   = 'M',
   WEAPONS    = 'X', 
   MEDICINE   = '+', 
   DRUGS      = 'D', 
   __RCOUNT__ = 12,
   NOTHING    = '.',
};

extern array<Resource, Resource::__RCOUNT__> ResourceList;

string resource_name(Resource r);

#endif  // ENGINE_RESOURCES_H_

// vim: ts=4:sw=4:sts=4:expandtab
