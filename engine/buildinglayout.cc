#include "engine/buildinglayout.h"

std::map<BuildingPair, BuildingLayout> BuildingLayouts = {
{ 
	BuildingPair(BuildingType::GENERAL_STORE, 1), 
	{
		BuildingLayout(8, 5, 4,
		{
			{
				"        ",
				" ffffff ",
				" ffffff ",
				" ffffff ",
				"        ",
			},{

				"wwwwwwww",
				"w      w",
				"w      w",
				"w      w",
				"wwwwddww",
			},{
				"wwwwwwww",
				"w      w",
				"w      w",
				"w      w",
				"wwwwddww",
			},{
				"wwwwwwww",
				"wwwwwwww",
				"wwwwwwww",
				"wwwwwwww",
				"wwwwwwww",
			},
		},
		{
			{ ' ', Block::EMPTY },
			{ 'f', Block::FLOOR },
			{ 'w', Block::WOODEN_WALL },
			{ 'd', Block::DOOR_OPEN },
		}),
	},
},
};
