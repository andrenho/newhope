#include "world/buildingtype.h"

/* w1..9 = house wall
 * d1..2 = house door
 */

const BuildingType BuildingType::BANK = BuildingType(7, 4,
		{ "w1w2w2        ",
		  "w4w5w5w1w2w2w3",
		  "w7w8w8w4w5d1w6",
		  "      w7w8d2w9" });
