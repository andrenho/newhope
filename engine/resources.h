#ifndef ENGINE_RESOURCES_H_
#define ENGINE_RESOURCES_H_

#include <array>

enum Resource {
   GRAIN, WOOD, OIL, HERBS, SILICON, IRON, URANIUM, ELECRONICS, MACHINES,
   WEAPONS, MEDICINE, DRUGS, __RCOUNT__
};

extern std::array<Resource, Resource::__RCOUNT__> ResourceList;

#endif  // ENGINE_RESOURCES_H_

// vim: ts=4:sw=4:sts=4:expandtab
