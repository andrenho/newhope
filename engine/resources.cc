#include "engine/resources.h"

#include <cstdlib>

#include "./globals.h"

std::array<Resource, Resource::__RCOUNT__> ResourceList {{ 
   GRAIN, WOOD, OIL, HERBS, SILICON, IRON, URANIUM, ELECRONICS, MACHINES,
   WEAPONS, MEDICINE, DRUGS,
}};

std::string resource_name(Resource r)
{
    switch(r) {
	case Resource::GRAIN:
        return _("Grain");
	case Resource::WOOD:
        return _("Wood");
	case Resource::OIL:
        return _("Oil");
	case Resource::HERBS:
        return _("Herbs");
	case Resource::SILICON:
        return _("Silicon");
	case Resource::IRON:
        return _("Iron");
	case Resource::URANIUM:
        return _("Uranium");
	case Resource::ELECRONICS:
        return _("Electronics");
	case Resource::MACHINES:
        return _("Machines");
	case Resource::WEAPONS:
        return _("Weapons");
	case Resource::MEDICINE:
        return _("Medicine");
    case Resource::DRUGS:
        return _("Drugs");
    default:
        abort();
    }
}

// vim: ts=4:sw=4:sts=4:expandtab
