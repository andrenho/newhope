// Copyright 2014 <Imperial Software>

#include "engine/buildinglayout.h"

BuildingLayout BuildingLayouts[] = {
	{
		8, 5, 4,
		BuildingType::GENERAL_STORE,
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
		},
	},
};
