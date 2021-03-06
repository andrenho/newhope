#include "engine/resources.h"

#include <cstdlib>
#include <string>
using namespace std;


#include "./main.h"

array<Resource, Resource::__RCOUNT__> ResourceList {{ 
   GRAIN, WOOD, OIL, HERBS, SILICON, IRON, URANIUM, ELECRONICS, MACHINES,
   WEAPONS, MEDICINE, DRUGS,
}};

string resource_name(Resource r)
{
    switch(r) {
	case Resource::GRAIN:
        return _("grain");
	case Resource::WOOD:
        return _("wood");
	case Resource::OIL:
        return _("oil");
	case Resource::HERBS:
        return _("herbs");
	case Resource::SILICON:
        return _("silicon");
	case Resource::IRON:
        return _("iron");
	case Resource::URANIUM:
        return _("uranium");
	case Resource::ELECRONICS:
        return _("electronics");
	case Resource::MACHINES:
        return _("machines");
	case Resource::WEAPONS:
        return _("weapons");
	case Resource::MEDICINE:
        return _("medicine");
    case Resource::DRUGS:
        return _("drugs");
    case NOTHING: case __RCOUNT__: default: // satisfy compiler...
        abort();
    }
}

// vim: ts=4:sw=4:sts=4:expandtab
