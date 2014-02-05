#include "engine/citylayout.h"

std::map<CityPair, CityLayout> CityLayouts = {
{
	CityPair(CityType::AGRICULTURAL, 1),
	{
		CityLayout(70, 20,
		{
			BuildingPosition(BuildingPair(BuildingType::GENERAL_STORE, 1), Point(0, 0)),
		}),
	},
},
};
